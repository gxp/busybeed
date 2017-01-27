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
int				 my_pfd;

char				 default_port[6];
char				*bind_interface = NULL;
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
#line 82 "y.tab.c"
#define BAUD 257
#define DATA 258
#define PARITY 259
#define STOP 260
#define HARDWARE 261
#define SOFTWARE 262
#define PASSWORD 263
#define NAME 264
#define RETRY 265
#define PERSISTENT 266
#define LOG 267
#define VERBOSE 268
#define CONNECT 269
#define DEVICE 270
#define LISTEN 271
#define LOCATION 272
#define IPADDR 273
#define DEVICES 274
#define CONNECTION 275
#define DEFAULT 276
#define PORT 277
#define MAX 278
#define CLIENTS 279
#define SUBSCRIPTIONS 280
#define BIND 281
#define INTERFACE 282
#define SUBSCRIBE 283
#define ERROR 284
#define STRING 285
#define NUMBER 286
#define YYERRCODE 256
const short yylhs[] =
	{                                        -1,
    0,    0,    0,    0,    0,    1,    1,    1,    1,    1,
    1,    1,    2,   10,    9,   11,   12,   13,   13,   14,
   15,   15,    6,    5,   16,    3,   17,    8,    4,   18,
   18,   20,   20,   21,   21,   21,   21,   21,   21,   21,
   21,   21,   21,   23,   23,   24,   24,   24,   24,   24,
   24,   25,   25,   27,   26,   28,   26,   29,    7,   19,
   19,   22,
};
const short yylen[] =
	{                                         2,
    0,    2,    3,    2,    3,    3,    1,    1,    1,    1,
    1,    1,    4,    0,    8,    3,    4,    2,    1,    7,
    2,    0,    3,    3,    3,    3,    3,    3,    3,    0,
    4,    3,    2,    4,    2,    2,    2,    2,    2,    2,
    2,    1,    1,    3,    2,    4,    4,    2,    2,    1,
    1,    3,    2,    0,    4,    0,    7,    0,    7,    2,
    0,    2,
};
const short yydefred[] =
	{                                      1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    2,
    0,    4,    7,    8,    9,   10,   11,   12,    5,    0,
   58,    0,    0,    0,    0,    0,   14,    3,   23,    0,
   28,    6,   26,   29,   24,    0,    0,    0,   13,    0,
    0,    0,    0,    0,   60,    0,    0,    0,    0,    0,
    0,   54,   56,   59,    0,   53,   16,    0,    0,    0,
    0,   52,    0,    0,   55,    0,   62,    0,    0,    0,
    0,    0,   15,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   42,   43,    0,    0,    0,    0,    0,
    0,   50,   51,    0,    0,    0,    0,   19,   35,   36,
   37,   38,   39,   40,   41,    0,    0,    0,   31,    0,
   33,   48,   49,    0,    0,   57,    0,   45,    0,   17,
   18,    0,   27,   25,   32,    0,    0,   44,    0,   34,
   47,   46,    0,    0,    0,    0,   20,   21,
};
const short yydgoto[] =
	{                                       1,
   11,   12,   13,   14,   15,   16,   17,   18,   36,   37,
   44,   69,   97,   98,  137,   84,   85,   65,   42,   86,
   87,   62,   94,   95,   48,   49,   59,   60,   30,
};
const short yysindex[] =
	{                                      0,
  -10,   14, -234, -248, -230, -239, -270, -235,  -75,    0,
   39,    0,    0,    0,    0,    0,    0,    0,    0, -236,
    0, -231, -228, -224, -219, -229,    0,    0,    0,  -54,
    0,    0,    0,    0,    0,  -55,  -50,   67,    0, -186,
   67, -250,   35,  -45,    0, -204, -203, -121,   67, -201,
   41,    0,    0,    0,   77,    0,    0,  -35,  -34,  -33,
   67,    0, -183,   67,    0,   67,    0,  -31,  -32, -242,
 -206, -176,    0, -191, -190, -188, -187, -185, -184, -182,
 -181, -179, -177,    0,    0,  -98,   67, -178, -180, -175,
 -174,    0,    0,  -72,   67,  -25, -114,    0,    0,    0,
    0,    0,    0,    0,    0, -169, -173, -171,    0,   77,
    0,    0,    0, -168, -165,    0,   77,    0, -170,    0,
    0, -167,    0,    0,    0, -166, -164,    0,   73,    0,
    0,    0, -162,   -7,   80,   80,    0,    0,};
const short yyrindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -240,    0,    0,
 -123,    0,    0,    0,    0,    0,    0,    0, -119,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -9,    0,
 -123,    0,    0, -217,    0, -195,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -73,    0,    0,    0,
    0,    0,    0,    0,  -59,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -113, -113,    0,    0,};
const short yygindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   24,  -11,  -68,  -63,    0,  -36,    0,
   40, -103,    0,   33,    0,   81,    0,    0,    0,
};
#define YYTABLESIZE 273
const short yytable[] =
	{                                      10,
   30,   61,   92,   54,   45,   61,  125,   93,   24,   25,
  120,   22,   56,  128,   74,   75,   76,   77,   78,   79,
   80,   46,   47,   19,   67,   92,  109,   70,   81,   71,
   93,   61,   61,   20,   22,   82,   21,   23,   83,   61,
   61,   61,   61,   61,   61,   61,   26,   27,   28,   29,
  111,   61,  116,   61,   31,   35,   88,   32,  118,   89,
   61,   33,   90,   61,   91,   61,   34,   61,   38,   39,
   61,   82,   40,   61,   83,   61,   41,   43,   50,   51,
   52,   53,   61,   57,   58,   61,   61,   63,   64,   66,
   68,   72,   73,   96,   99,  100,  101,  119,  102,  107,
  103,  104,  105,  106,  108,  113,  112,  122,  126,  114,
  115,  127,  123,  124,  129,   30,  133,  135,  130,  131,
  121,  132,  134,  136,  138,  110,  117,    0,   55,    0,
    0,    0,    0,   61,   61,   61,   61,   61,   61,   61,
    0,    0,   61,    0,    0,   61,    0,   61,   61,   61,
   46,   47,   61,   61,   61,   96,   22,   61,   74,   75,
   76,   77,   78,   79,   80,    0,    0,    0,    0,    0,
    0,    0,   81,    0,    0,    0,    0,    0,    0,   82,
    0,    0,   83,   61,   61,   61,   61,   61,   61,   61,
   88,    0,    0,   89,    0,    0,   90,   61,   91,    0,
    0,    0,    0,   61,   61,   82,   61,   61,   83,   61,
    0,   61,    0,    0,    0,    0,    0,    0,   61,    0,
    0,   61,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    2,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    3,    0,    0,    4,
    0,    0,   30,   30,    5,    6,    0,    7,    0,    0,
    8,    0,    9,
};
const short yycheck[] =
	{                                      10,
   10,  125,   71,  125,   41,  125,  110,   71,  279,  280,
  125,  125,   49,  117,  257,  258,  259,  260,  261,  262,
  263,  272,  273,   10,   61,   94,  125,   64,  271,   66,
   94,  272,  273,  268,  265,  278,  285,  277,  281,  257,
  258,  259,  260,  261,  262,  263,  282,  123,   10,  286,
   87,  125,  125,  271,  286,  285,  263,  286,   95,  266,
  278,  286,  269,  281,  271,  125,  286,  263,  123,  125,
  266,  278,  123,  269,  281,  271,   10,  264,   44,  125,
  285,  285,  278,  285,   44,  281,   10,  123,  123,  123,
  274,  123,  125,  270,  286,  286,  285,  123,  286,  279,
  286,  286,  285,  285,  282,  286,  285,  277,  277,  285,
  285,  277,  286,  285,  285,  125,   44,  125,  286,  286,
   97,  286,  285,   44,  136,   86,   94,   -1,   48,   -1,
   -1,   -1,   -1,  257,  258,  259,  260,  261,  262,  263,
   -1,   -1,  266,   -1,   -1,  269,   -1,  271,  272,  273,
  272,  273,  272,  273,  278,  270,  270,  281,  257,  258,
  259,  260,  261,  262,  263,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,  278,
   -1,   -1,  281,  257,  258,  259,  260,  261,  262,  263,
  263,   -1,   -1,  266,   -1,   -1,  269,  271,  271,   -1,
   -1,   -1,   -1,  263,  278,  278,  266,  281,  281,  269,
   -1,  271,   -1,   -1,   -1,   -1,   -1,   -1,  278,   -1,
   -1,  281,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  267,   -1,   -1,  270,
   -1,   -1,  272,  273,  275,  276,   -1,  278,   -1,   -1,
  281,   -1,  283,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 286
#if YYDEBUG
const char * const yyname[] =
	{
"end-of-file",0,0,0,0,0,0,0,0,0,"'\\n'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'{'",0,"'}'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"BAUD","DATA",
"PARITY","STOP","HARDWARE","SOFTWARE","PASSWORD","NAME","RETRY","PERSISTENT",
"LOG","VERBOSE","CONNECT","DEVICE","LISTEN","LOCATION","IPADDR","DEVICES",
"CONNECTION","DEFAULT","PORT","MAX","CLIENTS","SUBSCRIPTIONS","BIND",
"INTERFACE","SUBSCRIBE","ERROR","STRING","NUMBER",
};
const char * const yyrule[] =
	{"$accept : grammar",
"grammar :",
"grammar : grammar '\\n'",
"grammar : grammar main '\\n'",
"grammar : grammar dosub",
"grammar : grammar error '\\n'",
"main : DEFAULT PORT NUMBER",
"main : maxclients",
"main : maxsubs",
"main : bindopts1",
"main : logging",
"main : device",
"main : devretry",
"dosub : SUBSCRIBE '{' subopts '}'",
"$$1 :",
"subopts : $$1 '{' name '}' ',' '{' devices '}'",
"name : NAME ',' STRING",
"devices : DEVICES '{' subdevs2 '}'",
"subdevs2 : subdevs2 subdevs",
"subdevs2 : subdevs",
"subdevs : DEVICE '{' STRING ',' STRING '}' optcomma",
"optcomma : ',' optcomma",
"optcomma :",
"logging : LOG VERBOSE NUMBER",
"bindopts1 : BIND INTERFACE STRING",
"bindopts2 : BIND INTERFACE STRING",
"maxclients : MAX CLIENTS NUMBER",
"maxclientssub : MAX CLIENTS NUMBER",
"devretry : CONNECTION RETRY NUMBER",
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
"socopts1 : PERSISTENT NUMBER",
"socopts1 : bindopts2",
"socopts1 : maxclientssub",
"deviceopts2 : deviceopts2 deviceopts1 nl",
"deviceopts2 : deviceopts1 optnl",
"$$2 :",
"deviceopts1 : LOCATION STRING $$2 locopts",
"$$3 :",
"deviceopts1 : IPADDR STRING $$3 '{' optnl socopts2 '}'",
"$$4 :",
"device : DEVICE STRING $$4 '{' optnl deviceopts2 '}'",
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
#line 358 "parse.y"

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
	my_pfd =			 pfd;
	sclients =			 cconf;

	int			 errors = 0;
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
#line 796 "y.tab.c"
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
#line 103 "parse.y"
{ file->errors++; }
break;
case 6:
#line 105 "parse.y"
{
			snprintf(default_port, sizeof(default_port), "%d", yyvsp[0].v.number);
		}
break;
case 14:
#line 117 "parse.y"
{
			sub_reqs = 0;
		}
break;
case 16:
#line 121 "parse.y"
{
			TAILQ_FOREACH(sclient, &sclients->clients, entry) {
				if (sclient->pfd == my_pfd) {
					sclient->name = yyvsp[0].v.string;
					sclient->lastelement = 0;
					break;
				}
			}
		}
break;
case 20:
#line 136 "parse.y"
{
			if (sub_reqs < max_subscriptions) {
				TAILQ_FOREACH(sclient, &sclients->clients,
					      entry) {
					if (sclient->subscribed == 1)
						/* client has 
						 * already subscribed
						 */
						continue;
				}
				TAILQ_FOREACH(ldevs, &s_devs->s_devices,
					      entry) {
					if (strcmp(ldevs->name, yyvsp[-4].v.string) == 0)
						if (strcmp(ldevs->password, yyvsp[-2].v.string)
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
			} else
				log_warnx("max subscription requests exceeded");
				/*
				 * no need to kill a client entirely for 
				 * trying to subscribe to more than max 
				 * subscriptions, so just ignore the rest
				 * 
				 * keep this note in case i change my mind
				 * 
				 * yyerror("max subscription requests exceeded");
				 * YYERROR;
				 */

			sub_reqs++;
		}
break;
case 23:
#line 179 "parse.y"
{
			conf->verbose = yyvsp[0].v.number;
		}
break;
case 24:
#line 183 "parse.y"
{
			bind_interface = yyvsp[0].v.string;
		}
break;
case 25:
#line 187 "parse.y"
{
			currentdevice->bind_interface = yyvsp[0].v.string;
		}
break;
case 26:
#line 191 "parse.y"
{
			max_clients = yyvsp[0].v.number;
		}
break;
case 27:
#line 195 "parse.y"
{
			currentdevice->max_clients = yyvsp[0].v.number;
		}
break;
case 28:
#line 199 "parse.y"
{
			if (yyvsp[0].v.number >= 30 && yyvsp[0].v.number <= 600)
				c_retry = yyvsp[0].v.number;
			else
				c_retry = DEFAULTRETRY;
		}
break;
case 29:
#line 206 "parse.y"
{
			max_subscriptions = yyvsp[0].v.number;
		}
break;
case 34:
#line 216 "parse.y"
{
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", yyvsp[0].v.number);
		}
break;
case 35:
#line 220 "parse.y"
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
case 36:
#line 233 "parse.y"
{
			if (yyvsp[0].v.number > 8 || yyvsp[0].v.number < 5) {
				yyerror("data bits syntax error");
				YYERROR;
			} else
				currentdevice->databits = yyvsp[0].v.number;
		}
break;
case 37:
#line 240 "parse.y"
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
case 38:
#line 252 "parse.y"
{
			if (yyvsp[0].v.number > 2 || yyvsp[0].v.number < 1) {
				yyerror("stop bits syntax error");
				YYERROR;
			} else
				currentdevice->stopbits = yyvsp[0].v.number;
		}
break;
case 39:
#line 259 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("hardware syntax error");
				YYERROR;
			} else
				currentdevice->hwctrl = yyvsp[0].v.number;
		}
break;
case 40:
#line 266 "parse.y"
{
			if (yyvsp[0].v.number > 1 || yyvsp[0].v.number < 0) {
				yyerror("software syntax error");
				YYERROR;
			} else
				currentdevice->swctrl = yyvsp[0].v.number;
		}
break;
case 41:
#line 273 "parse.y"
{
			currentdevice->password = yyvsp[0].v.string;
		}
break;
case 46:
#line 282 "parse.y"
{
			snprintf(currentdevice->port,
				sizeof(currentdevice->port), "%d", yyvsp[0].v.number);
		}
break;
case 47:
#line 286 "parse.y"
{
			currentdevice->cport = yyvsp[0].v.number;
		}
break;
case 48:
#line 289 "parse.y"
{
			currentdevice->password = yyvsp[0].v.string;
		}
break;
case 49:
#line 292 "parse.y"
{
			currentdevice->persistent = yyvsp[0].v.number;
		}
break;
case 54:
#line 301 "parse.y"
{
			currentdevice->devicelocation = yyvsp[0].v.string;
		}
break;
case 56:
#line 304 "parse.y"
{
			currentdevice->ipaddr = yyvsp[0].v.string;
		}
break;
case 58:
#line 308 "parse.y"
{
			currentdevice =				 new_device(yyvsp[0].v.string);
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
		}
break;
case 59:
#line 325 "parse.y"
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
#line 1276 "y.tab.c"
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
