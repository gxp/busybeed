#include <stdlib.h>
#include <string.h>
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYLEX yylex()
#define YYEMPTY -1
#define yyclearin (yychar=(YYEMPTY))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING() (yyerrflag!=0)
#define YYPREFIX "yy"
#line 21 "parse.y"
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
int		 popfile(void);
int		 yyparse(void);
int		 yylex(void);
int		 yyerror(const char *, ...)
    __attribute__((__format__ (printf, 1, 2)))
    __attribute__((__nonnull__ (1)));
int		 kw_cmp(const void *, const void *);
int		 lookup(char *);
int		 lgetc(int);
int		 lungetc(int);
int		 findeol(void);

struct busybeed_conf		*conf;
char				 default_port[6];
char				*bind_interface = NULL;
extern int			 max_clients, max_subscriptions, verbose;
const char			*parity[4] = {"none", "odd", "even", "space"};
const int			 baudrates[18] = {50, 75, 110, 134, 150, 200,
						  300, 600, 1200, 1800, 2400,
						  4800, 9600, 38400, 57600,
						  76800, 115200};
const int			 s_parity =
					(sizeof(parity)/sizeof(const char *));
const int			 c_bauds =
					(sizeof(baudrates)/sizeof(const int));
int				 p_c = 0;
int				 b_c = 0;

typedef struct {
	union {
		int			 number;
		char			*string;
	} v;
	int lineno;
} YYSTYPE;
#line 73 "y.tab.c"
#define BAUD 257
#define DATA 258
#define PARITY 259
#define STOP 260
#define HARDWARE 261
#define SOFTWARE 262
#define PASSWORD 263
#define LOG 264
#define VERBOSE 265
#define CONNECT 266
#define DEVICE 267
#define LISTEN 268
#define LOCATION 269
#define IPADDR 270
#define DEFAULT 271
#define PORT 272
#define MAX 273
#define CLIENTS 274
#define SUBSCRIPTIONS 275
#define BIND 276
#define INTERFACE 277
#define ERROR 278
#define STRING 279
#define NUMBER 280
#define YYERRCODE 256
const short yylhs[] =
	{                                        -1,
    0,    0,    0,    0,    1,    1,    1,    1,    1,    1,
    5,    4,    7,    2,    8,    3,    9,    9,   11,   11,
   12,   12,   12,   12,   12,   12,   12,   12,   12,   12,
   14,   14,   15,   15,   15,   15,   16,   16,   18,   17,
   19,   17,   20,    6,   10,   10,   13,
};
const short yylen[] =
	{                                         2,
    0,    2,    3,    3,    3,    1,    1,    1,    1,    1,
    3,    3,    3,    3,    3,    3,    0,    4,    3,    2,
    4,    2,    2,    2,    2,    2,    2,    2,    1,    1,
    3,    2,    4,    4,    2,    1,    3,    2,    0,    4,
    0,    7,    0,    7,    2,    0,    2,
};
const short yydefred[] =
	{                                      1,
    0,    0,    0,    0,    0,    0,    0,    2,    0,    6,
    7,    8,    9,   10,    4,    0,   43,    0,    0,    0,
    0,    3,   11,    0,    5,   14,   16,   12,    0,    0,
    0,   45,    0,    0,    0,    0,   39,   41,   44,    0,
   38,    0,    0,    0,   37,    0,   40,    0,   47,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   29,   30,    0,    0,    0,    0,    0,   36,    0,
    0,   22,   23,   24,   25,   26,   27,   28,    0,    0,
    0,   18,    0,   20,   35,    0,    0,   42,    0,   32,
    0,   15,   13,   19,    0,    0,   31,   21,   34,   33,
};
const short yydgoto[] =
	{                                       1,
    9,   10,   11,   12,   13,   14,   62,   63,   47,   31,
   64,   65,   45,   70,   71,   35,   36,   42,   43,   24,
};
const short yysindex[] =
	{                                      0,
  -10,    1, -246, -233, -236, -261, -222,    0,   46,    0,
    0,    0,    0,    0,    0, -223,    0, -220, -219, -218,
 -221,    0,    0,  -60,    0,    0,    0,    0,   55,   55,
 -245,    0, -213, -211, -122,   55,    0,    0,    0,   59,
    0,  -53,  -52,   55,    0,   55,    0,   55,    0, -229,
 -256, -208, -207, -205, -204, -203, -202, -200, -199, -193,
 -195,    0,    0, -104,   55, -196, -194, -192,    0, -103,
   55,    0,    0,    0,    0,    0,    0,    0, -197, -191,
 -189,    0,   59,    0,    0, -188, -186,    0,   59,    0,
 -187,    0,    0,    0, -185, -184,    0,    0,    0,    0,};
const short yyrindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -243, -124,
    0,    0,    0,    0,    0, -119,    0,    0,    0,    0,
    0,   -5,    0, -124,    0, -209,    0, -228,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -84,    0,    0,    0,    0,    0,
  -83,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,};
const short yygindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,  -47,    0,  -28,
    0,   24,  -74,    0,   21,    0,   57,    0,    0,    0,
};
#define YYTABLESIZE 266
const short yytable[] =
	{                                       8,
   46,   32,   39,   69,   17,   46,   66,   41,   94,   67,
   15,   68,   19,   20,   97,   49,   60,   50,   16,   51,
   82,   88,   69,   33,   34,   46,   46,   52,   53,   54,
   55,   56,   57,   58,   46,   18,   84,   46,   59,   46,
   46,   46,   90,   60,   46,   17,   61,   46,   46,   46,
   46,   46,   46,   46,   21,   22,   23,   28,   46,   25,
   26,   27,   29,   46,   30,   37,   46,   38,   44,   46,
   48,   72,   73,   74,   91,   75,   76,   77,   78,   79,
   80,   81,   85,   95,   86,   96,   87,   83,   92,   93,
   89,   40,   98,    0,   99,  100,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   17,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   46,   46,   46,   46,   46,   46,   46,    0,
    0,   46,    0,   46,   46,   46,   33,   34,   46,   46,
   46,   46,   52,   53,   54,   55,   56,   57,   58,   66,
    0,    0,   67,   59,   68,    0,    0,    0,   60,   60,
    0,   61,   46,   46,   46,   46,   46,   46,   46,   46,
    0,    0,   46,   46,   46,    0,    0,    0,   46,   46,
    0,   46,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    2,    0,    0,    0,    0,
    0,    0,    0,    3,    0,    0,    4,    0,    0,    0,
    5,    0,    6,   17,   17,    7,
};
const short yycheck[] =
	{                                      10,
  125,   30,  125,   51,   10,  125,  263,   36,   83,  266,
   10,  268,  274,  275,   89,   44,  273,   46,  265,   48,
  125,  125,   70,  269,  270,  269,  270,  257,  258,  259,
  260,  261,  262,  263,  263,  272,   65,  266,  268,  268,
  125,  125,   71,  273,  273,  279,  276,  257,  258,  259,
  260,  261,  262,  263,  277,   10,  280,  279,  268,  280,
  280,  280,  123,  273,   10,  279,  276,  279,   10,  123,
  123,  280,  280,  279,  272,  280,  280,  280,  279,  279,
  274,  277,  279,  272,  279,  272,  279,   64,  280,  279,
   70,   35,  280,   -1,  280,  280,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  125,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,  261,  262,  263,   -1,
   -1,  266,   -1,  268,  269,  270,  269,  270,  273,  269,
  270,  276,  257,  258,  259,  260,  261,  262,  263,  263,
   -1,   -1,  266,  268,  268,   -1,   -1,   -1,  273,  273,
   -1,  276,  257,  258,  259,  260,  261,  262,  263,  263,
   -1,   -1,  266,  268,  268,   -1,   -1,   -1,  273,  273,
   -1,  276,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  264,   -1,   -1,  267,   -1,   -1,   -1,
  271,   -1,  273,  269,  270,  276,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 280
#if YYDEBUG
const char * const yyname[] =
	{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"BAUD","DATA",
"PARITY","STOP","HARDWARE","SOFTWARE","PASSWORD","LOG","VERBOSE","CONNECT",
"DEVICE","LISTEN","LOCATION","IPADDR","DEFAULT","PORT","MAX","CLIENTS",
"SUBSCRIPTIONS","BIND","INTERFACE","ERROR","STRING","NUMBER",
};
const char * const yyrule[] =
	{"$accept : grammar",
"grammar :",
"grammar : grammar '\\n'",
"grammar : grammar main '\\n'",
"grammar : grammar error '\\n'",
"main : DEFAULT PORT NUMBER",
"main : maxclients",
"main : maxsubs",
"main : bindopts1",
"main : logging",
"main : device",
"logging : LOG VERBOSE NUMBER",
"bindopts1 : BIND INTERFACE STRING",
"bindopts2 : BIND INTERFACE STRING",
"maxclients : MAX CLIENTS NUMBER",
"maxclientssub : MAX CLIENTS NUMBER",
"maxsubs : MAX SUBSCRIPTIONS NUMBER",
"locopts :",
"locopts : '{' optnl locopts2 '}'",
"locopts2 : locopts2 locopts1 nl",
"locopts2 : locopts1 optnl",
"locopts1 : LISTEN STRING PORT NUMBER",
"locopts1 : BAUD NUMBER",
"locopts1 : DATA NUMBER",
"locopts1 : PARITY STRING",
"locopts1 : STOP NUMBER",
"locopts1 : HARDWARE NUMBER",
"locopts1 : SOFTWARE NUMBER",
"locopts1 : PASSWORD STRING",
"locopts1 : bindopts2",
"locopts1 : maxclientssub",
"socopts2 : socopts2 socopts1 nl",
"socopts2 : socopts1 optnl",
"socopts1 : LISTEN STRING PORT NUMBER",
"socopts1 : CONNECT STRING PORT NUMBER",
"socopts1 : PASSWORD STRING",
"socopts1 : maxclientssub",
"deviceopts2 : deviceopts2 deviceopts1 nl",
"deviceopts2 : deviceopts1 optnl",
"$$1 :",
"deviceopts1 : LOCATION STRING $$1 locopts",
"$$2 :",
"deviceopts1 : IPADDR STRING $$2 '{' optnl socopts2 '}'",
"$$3 :",
"device : DEVICE STRING $$3 '{' optnl deviceopts2 '}'",
"optnl : '\\n' optnl",
"optnl :",
"nl : '\\n' optnl",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH 10000
#endif
#endif
#define YYINITSTACKSIZE 200
/* LINTUSED */
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short *yyss;
short *yysslim;
YYSTYPE *yyvs;
unsigned int yystacksize;
int yyparse(void);
#line 278 "parse.y"

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
		{ "baud",		BAUD},
		{ "bind",		BIND},
		{ "clients",		CLIENTS},
		{ "connect",		CONNECT},
		{ "data",		DATA},
		{ "default",		DEFAULT},
		{ "device",		DEVICE},
		{ "hardware",		HARDWARE},
		{ "interface",		INTERFACE},
		{ "ipaddr",		IPADDR},
		{ "listen",		LISTEN},
		{ "location",		LOCATION},
		{ "log",		LOG},
		{ "max",		MAX},
		{ "parity",		PARITY},
		{ "password",		PASSWORD},
		{ "port",		PORT},
		{ "software",		SOFTWARE},
		{ "stop",		STOP},
		{ "subscriptions",	SUBSCRIPTIONS},
		{ "verbose",		VERBOSE}
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
		/* Read character from the parsebuffer instead of input. */
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
		if ((c = getc(file->stream)) == EOF) {
			yyerror("reached end of file while parsing "
			    "quoted string");
			if (file == topfile || popfile() == EOF)
				return (EOF);
			return (quotec);
		}
		return (c);
	}

	while ((c = getc(file->stream)) == '\\') {
		next = getc(file->stream);
		if (next != '\n') {
			c = next;
			break;
		}
		yylval.lineno = file->lineno;
		file->lineno++;
	}

	while (c == EOF) {
		if (file == topfile || popfile() == EOF)
			return (EOF);
		c = getc(file->stream);
	}
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
parse_config(const char *filename, struct busybeed_conf *xconf)
{
	int			 errors = 0;
	conf =			 xconf;
	conf->verbose = 	 verbose;

	TAILQ_INIT(&conf->devices);

	if ((file = pushfile(filename)) == NULL) {
		return (-1);
	}
	topfile = file;

	yyparse();
	errors = file->errors;
	popfile();

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
#line 668 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    unsigned int newsize;
    long sslen;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;
    sslen = yyssp - yyss;
#ifdef SIZE_MAX
#define YY_SIZE_MAX SIZE_MAX
#else
#define YY_SIZE_MAX 0xffffffffU
#endif
    if (newsize && YY_SIZE_MAX / newsize < sizeof *newss)
        goto bail;
    newss = yyss ? (short *)realloc(yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss); /* overflow check above */
    if (newss == NULL)
        goto bail;
    yyss = newss;
    yyssp = newss + sslen;
    if (newsize && YY_SIZE_MAX / newsize < sizeof *newvs)
        goto bail;
    newvs = yyvs ? (YYSTYPE *)realloc(yyvs, newsize * sizeof *newvs) :
      (YYSTYPE *)malloc(newsize * sizeof *newvs); /* overflow check above */
    if (newvs == NULL)
        goto bail;
    yyvs = newvs;
    yyvsp = newvs + sslen;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
bail:
    if (yyss)
            free(yyss);
    if (yyvs)
            free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return -1;
}

#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse(void)
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif /* YYDEBUG */

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#if defined(__GNUC__)
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#if defined(__GNUC__)
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 4:
#line 93 "parse.y"
{ file->errors++; }
break;
case 5:
#line 95 "parse.y"
{
			snprintf(default_port, sizeof(default_port), "%d", yyvsp[0].v.number);
		}
break;
case 11:
#line 104 "parse.y"
{
			conf->verbose = yyvsp[0].v.number;
		}
break;
case 12:
#line 108 "parse.y"
{
			bind_interface = yyvsp[0].v.string;
		}
break;
case 13:
#line 112 "parse.y"
{
			currentdevice->bind_interface = yyvsp[0].v.string;
		}
break;
case 14:
#line 116 "parse.y"
{
			max_clients = yyvsp[0].v.number;
		}
break;
case 15:
#line 120 "parse.y"
{
			currentdevice->max_clients = yyvsp[0].v.number;
		}
break;
case 16:
#line 124 "parse.y"
{
			max_subscriptions = yyvsp[0].v.number;
		}
break;
case 21:
#line 134 "parse.y"
{
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", yyvsp[0].v.number);
		}
break;
case 22:
#line 138 "parse.y"
{
			currentdevice->baud = -1;
			for (b_c = 0; b_c < c_bauds; b_c++) {
				if (yyvsp[0].v.number == baudrates[b_c]) {
					currentdevice->baud = yyvsp[0].v.number;
					continue;
				}
			}
			if (currentdevice->baud == -1) {
				yyerror("baud rate syntax error");
				YYERROR;
			}
		}
break;
case 23:
#line 151 "parse.y"
{
			if (yyvsp[0].v.number > 8 || yyvsp[0].v.number < 5) {
				yyerror("data bits syntax error");
				YYERROR;
			} else {
				currentdevice->databits = yyvsp[0].v.number;
			}
		}
break;
case 24:
#line 159 "parse.y"
{
			for (p_c = 0; p_c < s_parity; p_c++) {
				if (strcmp(yyvsp[0].v.string, parity[p_c]) == 0) {
					currentdevice->parity = yyvsp[0].v.string;
					continue;
				}
			}
			if (currentdevice->parity == '\0') {
				yyerror("parity syntax error");
				YYERROR;
			}
		}
break;
case 25:
#line 171 "parse.y"
{
			if (yyvsp[0].v.number > 2 || yyvsp[0].v.number < 1) {
				yyerror("stop bits syntax error");
				YYERROR;
			} else {
				currentdevice->stopbits = yyvsp[0].v.number;
			}
		}
break;
case 26:
#line 179 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("hardware syntax error");
				YYERROR;
			} else {
				currentdevice->hwctrl = yyvsp[0].v.number;
			}
		}
break;
case 27:
#line 187 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("software syntax error");
				YYERROR;
			} else {
				currentdevice->swctrl = yyvsp[0].v.number;
			}
		}
break;
case 28:
#line 195 "parse.y"
{
			currentdevice->password = yyvsp[0].v.string;
		}
break;
case 33:
#line 204 "parse.y"
{
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", yyvsp[0].v.number);
		}
break;
case 34:
#line 208 "parse.y"
{
			currentdevice->cport = yyvsp[0].v.number;
			/*snprintf(currentdevice->cport,
				 sizeof(currentdevice->cport), "%d", $4);*/
		}
break;
case 35:
#line 213 "parse.y"
{
			currentdevice->password = yyvsp[0].v.string;
		}
break;
case 39:
#line 221 "parse.y"
{
			currentdevice->devicelocation = yyvsp[0].v.string;
		}
break;
case 41:
#line 224 "parse.y"
{
			currentdevice->ipaddr = yyvsp[0].v.string;
		}
break;
case 43:
#line 228 "parse.y"
{
			currentdevice =				 new_device(yyvsp[0].v.string);
			currentdevice->devicelocation =		 NULL;
			currentdevice->ipaddr =			 NULL;
			currentdevice->max_clients =		 max_clients;
			strlcpy(currentdevice->port, default_port,
				sizeof(currentdevice->port));
			currentdevice->baud = 			 DEFAULT_BAUD;
			currentdevice->bind_interface =		 bind_interface;
			currentdevice->cport =			 -1;
			currentdevice->databits =		 -1;
			currentdevice->parity =			 NULL;
			currentdevice->stopbits =		 -1;
			currentdevice->hwctrl =			 -1;
			currentdevice->swctrl =			 -1;
			currentdevice->password =		 NULL;
		}
break;
case 44:
#line 244 "parse.y"
{
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
break;
#line 1079 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    if (yyss)
            free(yyss);
    if (yyvs)
            free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return (1);
yyaccept:
    if (yyss)
            free(yyss);
    if (yyvs)
            free(yyvs);
    yyss = yyssp = NULL;
    yyvs = yyvsp = NULL;
    yystacksize = 0;
    return (0);
}
