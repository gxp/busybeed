/* $OpenBSD: parse.y v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
/*
 * Copyright (c) 2016 Tracey Emery <tracey@traceyemery.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* I borrowed extensively from ntpd. Thank you! */

%{
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "busybeed.h"

TAILQ_HEAD(files, file)		 files = TAILQ_HEAD_INITIALIZER(files);
static struct file {
	TAILQ_ENTRY(file)	 entry;
	FILE			*stream;
	char			*name;
	int			 lineno;
	int			 errors;
} *file, *topfile;
struct file	*pushfile(const char *);
struct file	*pushbuff(u_char *);
int		 popfile(void);
int		 popbuff(void);
int		 yyparse(void);
int		 yylex(void);
int		 yy_flush_buffer(void); /* look up */

int		 yyerror(const char *, ...)
    __attribute__((__format__ (printf, 1, 2)))
    __attribute__((__nonnull__ (1)));
int		 kw_cmp(const void *, const void *);
int		 lookup(char *);
int		 lgetc(int);
int		 lungetc(int);
int		 findeol(void);

struct busybeed_conf		*conf;
struct s_device			*ldevs;
struct client_conf		*sclients;
struct client			*sclient;
int				 my_pfd, fail, subs;

char				 default_port[6];
char				*bind_interface = NULL;
char				*my_name, *not_my_name;

extern int			 max_clients, max_subscriptions, verbose;
extern int			 c_retry;
const char			*parity[4] = {"none", "odd", "even", "space"};
const int			 baudrates[18] = {50, 75, 110, 134, 150, 200,
				     300, 600, 1200, 1800, 2400, 4800, 9600,
				     38400, 57600, 76800, 115200};
const int			 s_parity = (sizeof(parity) / 
				     sizeof(const char *));
const int			 c_bauds = (sizeof(baudrates) / 
				     sizeof(const int));
int				 p_c = 0, b_c = 0, sub_reqs = 0;

typedef struct {
	union {
		int			 number;
		char			*string;
	} v;
	int lineno;
} YYSTYPE;
%}

%token	BAUD DATA PARITY STOP HARDWARE SOFTWARE PASSWORD NAME RETRY PERSISTENT
%token	LOG VERBOSE CONNECT DEVICE LISTEN LOCATION IPADDR DEVICES CONNECTION
%token	DEFAULT PORT MAX CLIENTS SUBSCRIPTIONS BIND INTERFACE SUBSCRIBE
%token	ERROR
%token	<v.string>		STRING
%token	<v.number>		NUMBER

%%
grammar		: /* empty */
		| grammar '\n'
		| grammar main '\n'
		| grammar dosub
		| grammar error '\n' { file->errors++; }
		;
main		: DEFAULT PORT NUMBER {
			snprintf(default_port, sizeof(default_port), "%d", $3);
		}
		| maxclients
		| maxsubs
		| bindopts1
		| logging
		| device
		| devretry
		;
dosub		: SUBSCRIBE '{' subopts '}'
		;
subopts		: {
			sub_reqs = 0;
		} '{' name '}' ',' '{' devices '}'
		;
name		: NAME ',' STRING {
			TAILQ_FOREACH(sclient, &sclients->clients, entry) {
				if (sclient->pfd == my_pfd) {
					if (sclient->subscribed == 0) {
						sclient->name = $3;
						my_name = $3;
						not_my_name = $3;
					} else
						my_name = sclient->name;
						not_my_name = $3;
					break;
				}
			}
		}
		;
devices		: DEVICES '{' subdevs2 '}'
		;
subdevs2	: subdevs2 subdevs
		| subdevs
		;
subdevs		: DEVICE '{' STRING ',' STRING '}' optcomma {
			char		*devname, *devport;
			int		 devfd;
			TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
				if (strcmp(ldevs->name, $3) == 0) {
					devname = ldevs->name;
					devfd = ldevs->fd;
					devport = ldevs->port;
					break;
				}
			}
			TAILQ_FOREACH(sclient, &sclients->clients, entry) {
				fail = 0;
				/*
				 * check against existing name
				 */
				if (strcmp(sclient->name, my_name) == 0 &&
				    sclient->pfd != my_pfd) {
					fail = 1;
					break;
				}
				if (sclient->pfd == my_pfd) {
					/*
					 * check for subscriber existing name
					 */
					if (strcmp(my_name, not_my_name) != 0) {
						fail = 1;
						break;
					}
					/*
					 * check max subscriptions and
					 * that we aren't already subscribed
					 */
					if (sclient->subscribed >= 
					    max_subscriptions) {
						fail = 1;
						break;
					}
					for (subs = 0; subs < max_subscriptions;
					    subs++) {
						if (strcmp(devname, $3) == 0 &&
						    devfd ==
						    sclient->subscriptions[subs]
						    ) {
							fail = 1;
							break;
						}
					}
					/*
					 * check we're working on the
					 * right port
					 */
					if (strcmp(sclient->port, devport) != 0)
					{
						fail = 1;
						break;
					}
					break;
				} else
					continue;
			}
			TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
				if (fail)
					continue;
				if (strcmp(ldevs->name, $3) == 0)
					if (strcmp(ldevs->password, $5)
						== 0 && (ldevs->subscribers
						< ldevs->max_clients)) {
						ldevs->subscribers++;
						do_subscribe(my_pfd,
							ldevs->name,
							ldevs->fd,
							sclients);
						continue;
					}
			}
		}
		;
optcomma	: ',' optcomma
		| /* emtpy */
		;
logging		: LOG VERBOSE NUMBER {
			conf->verbose = $3;
		}
		;
bindopts1	: BIND INTERFACE STRING {
			bind_interface = $3;
		}
		;
bindopts2	: BIND INTERFACE STRING {
			currentdevice->bind_interface = $3;
		}
		;
maxclients	: MAX CLIENTS NUMBER {
			max_clients = $3;
		}
		;
maxclientssub	: MAX CLIENTS NUMBER {
			currentdevice->max_clients = $3;
		}
		;
devretry	: CONNECTION RETRY NUMBER {
			if ($3 >= 30 && $3 <= 600)
				c_retry = $3;
			else
				c_retry = DEFAULTRETRY;
		}
		;
maxsubs		: MAX SUBSCRIPTIONS NUMBER {
			max_subscriptions = $3;
		}
		;
locopts		: /* empty */
		|  '{' optnl locopts2 '}'
		;
locopts2	: locopts2 locopts1 nl
		| locopts1 optnl
		;
locopts1	: LISTEN STRING PORT NUMBER {
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", $4);
		}
		| BAUD NUMBER {
			currentdevice->baud = -1;
			for (b_c = 0; b_c < c_bauds; b_c++) {
				if ($2 == baudrates[b_c]) {
					currentdevice->baud = $2;
					continue;
				}
			}
			if (currentdevice->baud == -1) {
				yyerror("baud rate syntax error");
				YYERROR;
			}
		}
		| DATA NUMBER {
			if ($2 > 8 || $2 < 5) {
				yyerror("data bits syntax error");
				YYERROR;
			} else
				currentdevice->databits = $2;
		}
		| PARITY STRING {
			for (p_c = 0; p_c < s_parity; p_c++) {
				if (strcmp($2, parity[p_c]) == 0) {
					currentdevice->parity = $2;
					continue;
				}
			}
			if (currentdevice->parity == '\0') {
				yyerror("parity syntax error");
				YYERROR;
			}
		}
		| STOP NUMBER {
			if ($2 > 2 || $2 < 1) {
				yyerror("stop bits syntax error");
				YYERROR;
			} else
				currentdevice->stopbits = $2;
		}
		| HARDWARE NUMBER {
			if ($2 > 1 || $2 < 0) {
				yyerror("hardware syntax error");
				YYERROR;
			} else
				currentdevice->hwctrl = $2;
		}
		| SOFTWARE NUMBER {
			if ($2 > 1 || $2 < 0) {
				yyerror("software syntax error");
				YYERROR;
			} else
				currentdevice->swctrl = $2;
		}
		| PASSWORD STRING {
			currentdevice->password = $2;
		}
		| bindopts2
		| maxclientssub
		;
socopts2	: socopts2 socopts1 nl
		| socopts1 optnl
		;
socopts1	: LISTEN STRING PORT NUMBER {
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", $4);
		}
		| CONNECT STRING PORT NUMBER {
			currentdevice->cport = $4;
		}
		| PASSWORD STRING {
			currentdevice->password = $2;
		}
		| PERSISTENT NUMBER {
			currentdevice->persistent = $2;
		}
		| bindopts2
		| maxclientssub
		;
deviceopts2	: deviceopts2 deviceopts1 nl
		| deviceopts1 optnl
		;
deviceopts1	:  LOCATION STRING {
			currentdevice->devicelocation = $2;
		} locopts
		| IPADDR STRING {
			currentdevice->ipaddr = $2;
		} '{' optnl socopts2 '}'
		;
device		: DEVICE STRING	 {
			currentdevice =				 new_device($2);
			currentdevice->devicelocation =		 NULL;
			currentdevice->ipaddr =			 NULL;
			currentdevice->max_clients =		 max_clients;
			strlcpy(currentdevice->port, default_port,
				sizeof(currentdevice->port));
			currentdevice->baud = 			 DEFAULT_BAUD;
			currentdevice->bind_interface =		 NULL;
			currentdevice->cport =			 -1;
			currentdevice->databits =		 -1;
			currentdevice->parity =			 NULL;
			currentdevice->stopbits =		 -1;
			currentdevice->hwctrl =			 -1;
			currentdevice->swctrl =			 -1;
			currentdevice->password =		 "";
			currentdevice->persistent =		 1;
		} '{' optnl deviceopts2 '}' {
			if (currentdevice->ipaddr != '\0' &&
				currentdevice->cport == -1) {
				yyerror("ipaddr connect port empty");
				YYERROR;
			}
			if (currentdevice->ipaddr != '\0' &&
				currentdevice->devicelocation != '\0') {
				yyerror("too many device arguments");
				YYERROR;
			}
			if (*default_port == '\0') {
				yyerror("could not set default port");
				YYERROR;
			}
			if (max_clients < 1) {
				yyerror("could not set max clients");
				YYERROR;
			}
			if (max_subscriptions < 1) {
				yyerror("could not set max subscriptions");
				YYERROR;
			}
			TAILQ_INSERT_TAIL(&conf->devices, currentdevice, entry);
			currentdevice = NULL;
		}
		;
optnl		: '\n' optnl
		| /* empty */
		;
nl		: '\n' optnl
		;
%%

struct keywords {
	const char	*k_name;
	int		 k_val;
};

int
yyerror(const char *fmt, ...)
{
	va_list		 ap;
	char		*msg;

	file->errors++;
	va_start(ap, fmt);
	if (vasprintf(&msg, fmt, ap) == -1)
		fatalx("yyerror vasprintf");
	va_end(ap);
	log_warnx("%s:%d: %s", file->name, yylval.lineno, msg);
	free(msg);
	return (0);
}

int
kw_cmp(const void *k, const void *e)
{
	return (strcmp(k, ((const struct keywords *)e)->k_name));
}

int lookup(char *s) {

	/* this has to be sorted always */
	static const struct keywords keywords[] = {
		{"baud",		BAUD},
		{"bind",		BIND},
		{"clients",		CLIENTS},
		{"connect",		CONNECT},
		{"connection",		CONNECTION},
		{"data",		DATA},
		{"default",		DEFAULT},
		{"device",		DEVICE},
		{"devices",		DEVICES},
		{"hardware",		HARDWARE},
		{"interface",		INTERFACE},
		{"ipaddr",		IPADDR},
		{"listen",		LISTEN},
		{"location",		LOCATION},
		{"log",			LOG},
		{"max",			MAX},
		{"name",		NAME},
		{"parity",		PARITY},
		{"password",		PASSWORD},
		{"persistent",		PERSISTENT},
		{"port",		PORT},
		{"retry",		RETRY},
		{"software",		SOFTWARE},
		{"stop",		STOP},
		{"subscribe",		SUBSCRIBE},
		{"subscriptions",	SUBSCRIPTIONS},
		{"verbose",		VERBOSE}
	};
	const struct keywords	*p;

	p = bsearch(s, keywords, sizeof(keywords)/sizeof(keywords[0]),
	    sizeof(keywords[0]), kw_cmp);

	if (p)
		return (p->k_val);
	else
		return (STRING);
}

#define MAXPUSHBACK	128

u_char	*parsebuf;
int	 parseindex;
u_char	 pushback_buffer[MAXPUSHBACK];
int	 pushback_index = 0;

int
lgetc(int quotec)
{
	int		c, next;

	if (parsebuf) {
		/* Read character from the parsebuffer instead of file input */
		if (parseindex >= 0) {
			c = parsebuf[parseindex++];
			if (c != '\0')
				return (c);
			parsebuf = NULL;
		} else
			parseindex++;
	}

	if (pushback_index)
		return (pushback_buffer[--pushback_index]);

	if (quotec) {
		if (parseindex > 0) {
			/* malformed packet */
			yyerror("reached end of packet while parsing "
			"quoted string");
			return (-1);
		}
		if ((c = getc(file->stream)) == EOF) {
			yyerror("reached end of file while parsing "
			"quoted string");
			if (file == topfile || popfile() == EOF)
				return (EOF);
			return (quotec);
		}
		return (c);
	}

	if (parseindex == 0) {
		while ((c = getc(file->stream)) == '\\') {
			next = getc(file->stream);
			if (next != '\n') {
				c = next;
				break;
			}
			yylval.lineno = file->lineno;
			file->lineno++;
		}
	}

	while (c == EOF) {
		if (file == topfile || popfile() == EOF)
			return (EOF);
		c = getc(file->stream);
	}
	parseindex = 0;
	return (c);
}

int
lungetc(int c)
{
	if (c == EOF)
		return (EOF);
	if (parsebuf) {
		parseindex--;
		if (parseindex >= 0)
			return (c);
	}
	if (pushback_index < MAXPUSHBACK-1)
		return (pushback_buffer[pushback_index++] = c);
	else
		return (EOF);
}

int
findeol(void)
{
	int	c;

	parsebuf = NULL;

	/* skip to either EOF or the first real EOL */
	while (1) {
		if (pushback_index)
			c = pushback_buffer[--pushback_index];
		else
			c = lgetc(0);
		if (c == '\n') {
			file->lineno++;
			break;
		}
		if (c == EOF)
			break;
	}
	return (ERROR);
}

int
yylex(void)
{
	u_char	 buf[8096];
	u_char	*p;
	int	 quotec, next, c;
	int	 token;

	p = buf;
	while ((c = lgetc(0)) == ' ' || c == '\t')
		; /* nothing */

	yylval.lineno = file->lineno;
	if (c == '#')
		while ((c = lgetc(0)) != '\n' && c != EOF)
			; /* nothing */

	switch (c) {
	case '\'':
	case '"':
		quotec = c;
		while (1) {
			if ((c = lgetc(quotec)) == EOF)
				return (0);
			if (c == '\n') {
				file->lineno++;
				continue;
			} else if (c == '\\') {
				if ((next = lgetc(quotec)) == EOF)
					return (0);
				if (next == quotec || c == ' ' || c == '\t')
					c = next;
				else if (next == '\n') {
					file->lineno++;
					continue;
				} else
					lungetc(next);
			} else if (c == quotec) {
				*p = '\0';
				break;
			} else if (c == '\0') {
				yyerror("syntax error");
				return (findeol());
			}
			if (p + 1 >= buf + sizeof(buf) - 1) {
				yyerror("string too long");
				return (findeol());
			}
			*p++ = c;
		}
		yylval.v.string = strdup(buf);
		if (yylval.v.string == NULL)
			fatal("yylex: strdup");
		return (STRING);
	}

#define allowed_to_end_number(x) \
	(isspace(x) || x == ')' || x ==',' || x == '/' || x == '}' || x == '=')

	if (c == '-' || isdigit(c)) {
		do {
			*p++ = c;
			if ((unsigned)(p-buf) >= sizeof(buf)) {
				yyerror("string too long");
				return (findeol());
			}
		} while ((c = lgetc(0)) != EOF && isdigit(c));
		lungetc(c);
		if (p == buf + 1 && buf[0] == '-')
			goto nodigits;
		if (c == EOF || allowed_to_end_number(c)) {
			const char *errstr = NULL;

			*p = '\0';
			yylval.v.number = strtonum(buf, LLONG_MIN,
			    LLONG_MAX, &errstr);
			if (errstr) {
				yyerror("\"%s\" invalid number: %s",
				    buf, errstr);
				return (findeol());
			}
			return (NUMBER);
		} else {
nodigits:
			while (p > buf + 1)
				lungetc(*--p);
			c = *--p;
			if (c == '-')
				return (c);
		}
	}

#define allowed_in_string(x) \
	(isalnum(x) || (ispunct(x) && x != '(' && x != ')' && \
	x != '{' && x != '}' && x != '<' && x != '>' && \
	x != '!' && x != '=' && x != '/' && x != '#' && \
	x != ','))

	if (isalnum(c) || c == ':' || c == '_' || c == '*') {
		do {
			*p++ = c;
			if ((unsigned)(p-buf) >= sizeof(buf)) {
				yyerror("string too long");
				return (findeol());
			}
		} while ((c = lgetc(0)) != EOF && (allowed_in_string(c)));
		lungetc(c);
		*p = '\0';
		if ((token = lookup(buf)) == STRING)
			if ((yylval.v.string = strdup(buf)) == NULL)
				fatal("yylex: strdup");
		return (token);
	}
	if (c == '\n') {
		yylval.lineno = file->lineno;
		file->lineno++;
	}
	if (c == EOF)
		return (0);
	return (c);
}

struct file *
pushfile(const char *name)
{
	struct file	*nfile;

	if ((nfile = calloc(1, sizeof(struct file))) == NULL) {
		log_warn("malloc");
		return (NULL);
	}
	if ((nfile->name = strdup(name)) == NULL) {
		log_warn("malloc");
		free(nfile);
		return (NULL);
	}
	if ((nfile->stream = fopen(nfile->name, "r")) == NULL) {
		log_warn("%s", nfile->name);
		free(nfile->name);
		free(nfile);
		return (NULL);
	}
	nfile->lineno = 1;
	TAILQ_INSERT_TAIL(&files, nfile, entry);
	return (nfile);
}

struct file *
pushbuff(u_char *xbuff)
{
	struct file	*bfile;
	
	if ((bfile = calloc(1, sizeof(struct file))) == NULL) {
		log_warn("malloc");
		return (NULL);
	}
	if ((bfile->name = "subscribe buffer") == NULL) {
		log_warn("malloc");
		free(bfile);
		return (NULL);
	}
	if ((parsebuf = xbuff) == NULL) {
		log_warn("%s", bfile->name);
		free(bfile);
		return (NULL);
	}
	bfile->lineno = 1;
	TAILQ_INSERT_TAIL(&files, bfile, entry);
	return (bfile);
}

int
popfile(void)
{
	struct file	*prev;

	if ((prev = TAILQ_PREV(file, files, entry)) != NULL)
		prev->errors += file->errors;

	TAILQ_REMOVE(&files, file, entry);
	fclose(file->stream);
	free(file->name);
	free(file);
	file = prev;
	return (file ? 0 : EOF);
}

int
popbuff(void)
{
	struct file	*prev;

	if ((prev = TAILQ_PREV(file, files, entry)) != NULL)
		prev->errors += file->errors;

	TAILQ_REMOVE(&files, file, entry);

	free(file);
	file = prev;
	return (file ? 0 : EOF);
}

int
parse_config(const char *filename, struct busybeed_conf *xconf)
{
	
	int			 errors = 0;
	conf =			 xconf;
	conf->verbose = 	 verbose;

	TAILQ_INIT(&conf->devices);

	if ((file = pushfile(filename)) == NULL)
		return (-1);

	topfile = file;

	yyparse();

	errors = file->errors;
	popfile();

	return (errors ? -1 : 0);
}

int
parse_buffer(struct client_conf *cconf, u_char *xbuff, int pfd)
{
	my_pfd =		 pfd;
	sclients =		 cconf;

	int				 errors = 0;
	if ((file = pushbuff(xbuff)) == NULL)
		return (-1);

	topfile = file;

	yyparse();

	errors = file->errors;
	
	popbuff();

	return (errors ? -1 : 0);
}

struct device *
new_device(char *name)
{
	struct device	*dev;
	
	if ((dev = calloc(1, sizeof(*dev))) == NULL)
		fatalx("no dev calloc");
	
	if ((dev->name = strdup(name)) == NULL)
		fatalx("no dev name");
	
	return (dev);
};