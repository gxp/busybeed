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
char				*default_port = NULL;
extern int			 max_clients, max_subscriptions;
const char			*parity[4] = {"none", "odd", "even", "space"};
const int			 baudrates[18] = {50, 75, 110, 134, 150, 200,
							300, 600, 1200, 1800,
							2400, 4800, 9600, 38400,
							57600, 76800, 115200};
const int			 s_parity =
					(sizeof(parity)/sizeof(const char *));
const int			 c_bauds =
					(sizeof(baudrates)/sizeof(const int));
int				 p_c = 0;
int				 b_c = 0;

typedef struct {
	union {
		int64_t			 number;
		char			*string;
	} v;
	int lineno;
} YYSTYPE;
#line 72 "y.tab.c"
#define BAUD 257
#define DATA 258
#define PARITY 259
#define STOP 260
#define HARDWARE 261
#define SOFTWARE 262
#define PASSWORD 263
#define DEVICE 264
#define LISTEN 265
#define LOCATION 266
#define DEFAULT 267
#define PORT 268
#define MAX 269
#define CLIENTS 270
#define SUBSCRIPTIONS 271
#define ERROR 272
#define STRING 273
#define NUMBER 274
#define YYERRCODE 256
const short yylhs[] =
	{                                        -1,
    0,    0,    0,    0,    0,    2,    2,    2,    3,    3,
    4,    4,    4,    4,    4,    4,    4,    4,    7,    7,
    8,    8,   10,    9,   11,    1,    6,    6,    5,
};
const short yylen[] =
	{                                         2,
    0,    2,    3,    3,    3,    3,    3,    3,    3,    2,
    4,    2,    2,    2,    2,    2,    2,    2,    0,    4,
    3,    2,    0,    4,    0,    7,    2,    0,    2,
};
const short yydefred[] =
	{                                      1,
    0,    0,    0,    0,    0,    2,    0,    0,    5,   25,
    0,    0,    0,    3,    4,    0,    6,    7,    8,    0,
    0,    0,   27,    0,    0,    0,   23,   26,    0,   22,
    0,    0,   21,    0,   24,   29,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,   13,   14,
   15,   16,   17,   18,    0,   20,    0,   10,    0,    9,
   11,
};
const short yydgoto[] =
	{                                       1,
    7,    8,   46,   47,   33,   22,   35,   25,   26,   31,
   16,
};
const short yysindex[] =
	{                                      0,
  -10,   -2, -262, -259, -264,    0,    2,    3,    0,    0,
 -258, -257, -255,    0,    0, -103,    0,    0,    0,   11,
   11, -244,    0, -231, -122,   11,    0,    0,   30,    0,
  -80,   11,    0,   11,    0,    0, -233, -230, -229, -227,
 -226, -225, -223, -221, -220, -111,   11,    0,    0,    0,
    0,    0,    0,    0, -218,    0,   30,    0, -219,    0,
    0,};
const short yyrindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -211,
 -124,    0,    0,    0,    0, -121,    0,    0,    0,    0,
   -8, -124,    0, -224,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -102,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,};
const short yygindex[] =
	{                                      0,
    0,    0,    0,    1,   -1,  -16,    0,    0,   32,    0,
    0,
};
#define YYTABLESIZE 259
const short yytable[] =
	{                                       6,
   28,   19,   28,   28,   23,   12,   13,    9,   11,   30,
   10,   14,   15,   56,   17,   36,   18,   37,   19,   20,
   21,   24,   28,   38,   39,   40,   41,   42,   43,   44,
   58,   45,   28,   28,   28,   28,   28,   28,   28,   32,
   28,   27,   34,   48,   49,   50,   57,   51,   52,   59,
   53,   54,   55,   61,   28,   60,   29,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   19,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   28,   28,   28,   28,   28,   28,   28,    0,
   28,   28,    0,   24,   28,   38,   39,   40,   41,   42,
   43,   44,    0,   45,   28,   28,   28,   28,   28,   28,
   28,    0,   28,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    2,    0,    0,    0,    0,
    0,    0,    0,    3,    0,    0,    4,   19,    5,
};
const short yycheck[] =
	{                                      10,
  125,   10,  125,  125,   21,  270,  271,   10,  268,   26,
  273,   10,   10,  125,  273,   32,  274,   34,  274,  123,
   10,  266,  125,  257,  258,  259,  260,  261,  262,  263,
   47,  265,  257,  258,  259,  260,  261,  262,  263,   10,
  265,  273,  123,  274,  274,  273,   46,  274,  274,  268,
  274,  273,  273,  273,  266,   57,   25,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  125,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,  261,  262,  263,   -1,
  265,  266,   -1,  266,  266,  257,  258,  259,  260,  261,
  262,  263,   -1,  265,  257,  258,  259,  260,  261,  262,
  263,   -1,  265,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  264,   -1,   -1,  267,  266,  269,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 274
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
"PARITY","STOP","HARDWARE","SOFTWARE","PASSWORD","DEVICE","LISTEN","LOCATION",
"DEFAULT","PORT","MAX","CLIENTS","SUBSCRIPTIONS","ERROR","STRING","NUMBER",
};
const char * const yyrule[] =
	{"$accept : grammar",
"grammar :",
"grammar : grammar '\\n'",
"grammar : grammar device '\\n'",
"grammar : grammar main '\\n'",
"grammar : grammar error '\\n'",
"main : DEFAULT PORT STRING",
"main : MAX CLIENTS NUMBER",
"main : MAX SUBSCRIPTIONS NUMBER",
"locopts2 : locopts2 locopts1 nl",
"locopts2 : locopts1 optnl",
"locopts1 : LISTEN STRING PORT STRING",
"locopts1 : BAUD NUMBER",
"locopts1 : DATA NUMBER",
"locopts1 : PARITY STRING",
"locopts1 : STOP NUMBER",
"locopts1 : HARDWARE NUMBER",
"locopts1 : SOFTWARE NUMBER",
"locopts1 : PASSWORD STRING",
"locopts :",
"locopts : '{' optnl locopts2 '}'",
"deviceopts2 : deviceopts2 deviceopts1 nl",
"deviceopts2 : deviceopts1 optnl",
"$$1 :",
"deviceopts1 : LOCATION STRING $$1 locopts",
"$$2 :",
"device : DEVICE STRING $$2 '{' optnl deviceopts2 '}'",
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
#line 215 "parse.y"

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
		{ "clients",		CLIENTS},
		{ "data",		DATA},
		{ "default",		DEFAULT},
		{ "device",		DEVICE},
		{ "hardware",		HARDWARE},
		{ "listen",		LISTEN},
		{ "location",		LOCATION},
		{ "max",		MAX},
		{ "parity",		PARITY},
		{ "password",		PASSWORD},
		{ "port",		PORT},
		{ "software",		SOFTWARE},
		{ "stop",		STOP},
		{ "subscriptions",	SUBSCRIPTIONS}
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
	int		 errors = 0;
	conf =		 xconf;

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
#line 620 "y.tab.c"
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
case 5:
#line 93 "parse.y"
{ file->errors++; }
break;
case 6:
#line 95 "parse.y"
{
			default_port = yyvsp[0].v.string;
		}
break;
case 7:
#line 98 "parse.y"
{
			max_clients = yyvsp[0].v.number;
		}
break;
case 8:
#line 101 "parse.y"
{
			max_subscriptions = yyvsp[0].v.number;
		}
break;
case 11:
#line 108 "parse.y"
{
			currentdevice->port = yyvsp[0].v.string;
		}
break;
case 12:
#line 111 "parse.y"
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
case 13:
#line 124 "parse.y"
{
			if (yyvsp[0].v.number > 8 || yyvsp[0].v.number < 5) {
				yyerror("data bits syntax error");
				YYERROR;
			} else {
				currentdevice->databits = yyvsp[0].v.number;
			}
		}
break;
case 14:
#line 132 "parse.y"
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
case 15:
#line 144 "parse.y"
{
			if (yyvsp[0].v.number > 2 || yyvsp[0].v.number < 1) {
				yyerror("stop bits syntax error");
				YYERROR;
			} else {
				currentdevice->stopbits = yyvsp[0].v.number;
			}
		}
break;
case 16:
#line 152 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("hardware syntax error");
				YYERROR;
			} else {
				currentdevice->hwctrl = yyvsp[0].v.number;
			}
		}
break;
case 17:
#line 160 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("software syntax error");
				YYERROR;
			} else {
				currentdevice->swctrl = yyvsp[0].v.number;
			}
		}
break;
case 18:
#line 168 "parse.y"
{
			currentdevice->password = yyvsp[0].v.string;
		}
break;
case 23:
#line 178 "parse.y"
{
			currentdevice->devicelocation = yyvsp[0].v.string;
		}
break;
case 25:
#line 182 "parse.y"
{
			currentdevice = 		new_device(yyvsp[0].v.string);
			currentdevice->port =		default_port;
			currentdevice->baud = 		DEFAULT_BAUD;
			currentdevice->databits =	-1;
			currentdevice->parity =		NULL;
			currentdevice->stopbits =	-1;
			currentdevice->hwctrl =		-1;
			currentdevice->swctrl =		-1;
			currentdevice->password =	NULL;
		}
break;
case 26:
#line 192 "parse.y"
{
			if (default_port == '\0') {
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
#line 962 "y.tab.c"
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
