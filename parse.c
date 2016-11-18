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
#line 20 "parse.y"
#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <string.h>
#include <syslog.h>

#include "busybee.h"

TAILQ_HEAD(files, file)		 files = TAILQ_HEAD_INITIALIZER(files);
static struct file {
	TAILQ_ENTRY(file)	 entry;
	FILE			*stream;
	char			*name;
	int			 lineno;
	int			 errors;
} *file, *topfile;

static int			 errors = 0;
int				 default_port = 50000;

struct file	*pushfile(const char *, int);
int		 popfile(void);
int		 check_file_secrecy(int, const char *);
int		 yyparse(void);
int		 yylex(void);
int		 yyerror(const char *, ...);
	__attribute__((__format__ (printf, 1, 2)))
	__attribute__((__nonnull__ (1)));
int		 kw_cmp(const void *, const void *);
int		 lookup(char *);
int		 lgetc(int);
int		 lungetc(int);
int		 findeol(void);

TAILQ_HEAD(symhead, sym)	 symhead = TAILQ_HEAD_INITIALIZER(symhead);

struct sym {
	TAILQ_ENTRY(sym)	 entry;
	int			 used;
	int			 persist;
	char			*nam;
	char			*val;
};

int		 symset(const char *, const char *, int);
char		*symget(const char *);

typedef struct {
	union {
		int64_t			 number;
		char			*string;
	} v;
	int lineno;
} YYSTYPE;
#line 74 "y.tab.c"
#define BAUD 257
#define DATA 258
#define PARITY 259
#define STOP 260
#define HARDWARE 261
#define PASSWORD 262
#define LISTEN 263
#define LOCATION 264
#define DEVICE 265
#define DEFAULT 266
#define PORT 267
#define ERROR 268
#define STRING 269
#define NUMBER 270
#define YYERRCODE 256
const short yylhs[] =
	{                                        -1,
    0,    0,    0,    0,    0,    2,    3,    3,    4,    7,
    4,    4,    4,    4,    4,    4,    4,    8,    1,    6,
    6,    5,
};
const short yylen[] =
	{                                         2,
    0,    3,    2,    3,    3,    3,    3,    2,    4,    0,
    7,    2,    2,    2,    2,    2,    2,    0,    7,    2,
    0,    2,
};
const short yydefred[] =
	{                                      1,
    0,    0,    0,    0,    3,    0,    0,    5,   18,    0,
    2,    4,    0,    6,    0,    0,    0,   20,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   12,   13,
   14,   15,   16,   17,    0,   10,   19,    0,    8,    0,
    0,    0,    7,    9,    0,   22,    0,    0,   11,
};
const short yydgoto[] =
	{                                       1,
    6,    7,   27,   28,   43,   17,   41,   13,
};
const short yysindex[] =
	{                                      0,
  -10,   -6, -264, -261,    0,   -3,   -2,    0,    0, -260,
    0,    0, -112,    0,    2,    2, -225,    0, -257, -255,
 -253, -252, -251, -249, -248, -247, -124,    2,    0,    0,
    0,    0,    0,    0, -244,    0,    0,   15,    0, -243,
  -97,    2,    0,    0,    2,    0, -225, -116,    0,};
const short yyrindex[] =
	{                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0, -217, -108,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0, -108,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -108,    0,    0, -217,    0,    0,    0,    0,};
const short yygindex[] =
	{                                      0,
    0,    0,  -18,  -24,    0,  -14,    0,    0,
};
#define YYTABLESIZE 256
const short yytable[] =
	{                                       5,
   37,   18,   38,    8,    9,   10,   11,   12,   49,   14,
   15,   16,   29,   39,   30,   31,   21,   32,   33,   34,
   35,   36,   40,   38,   42,   45,   44,   46,   48,    0,
   47,   19,   20,   21,   22,   23,   24,   25,   26,   21,
   21,   21,   21,   21,   21,   21,   21,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   19,   20,   21,   22,   23,   24,   25,   26,
   19,   20,   21,   22,   23,   24,   25,   26,   21,   21,
   21,   21,   21,   21,   21,   21,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    2,    0,    0,    0,    0,
    0,    0,    0,    0,    3,    4,
};
const short yycheck[] =
	{                                      10,
  125,   16,   27,   10,  269,  267,   10,   10,  125,  270,
  123,   10,  270,   28,  270,  269,  125,  270,  270,  269,
  269,  269,  267,   48,   10,  123,  270,   42,   47,   -1,
   45,  257,  258,  259,  260,  261,  262,  263,  264,  257,
  258,  259,  260,  261,  262,  263,  264,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  257,  258,  259,  260,  261,  262,  263,  264,
  257,  258,  259,  260,  261,  262,  263,  264,  257,  258,
  259,  260,  261,  262,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  256,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  265,  266,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 270
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
"PARITY","STOP","HARDWARE","PASSWORD","LISTEN","LOCATION","DEVICE","DEFAULT",
"PORT","ERROR","STRING","NUMBER",
};
const char * const yyrule[] =
	{"$accept : grammar",
"grammar :",
"grammar : grammar device '\\n'",
"grammar : grammar '\\n'",
"grammar : grammar main '\\n'",
"grammar : grammar error '\\n'",
"main : DEFAULT PORT NUMBER",
"deviceopts2 : deviceopts2 deviceopts1 nl",
"deviceopts2 : deviceopts1 optnl",
"deviceopts1 : LISTEN STRING PORT NUMBER",
"$$1 :",
"deviceopts1 : LOCATION STRING $$1 '{' optnl deviceopts2 '}'",
"deviceopts1 : BAUD NUMBER",
"deviceopts1 : DATA NUMBER",
"deviceopts1 : PARITY STRING",
"deviceopts1 : STOP NUMBER",
"deviceopts1 : HARDWARE NUMBER",
"deviceopts1 : PASSWORD STRING",
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
#line 139 "parse.y"

/* additional c code */
struct keywords {
	const char	*k_name;
	int		 k_val;
};

int yyerror(const char *fmt, ...) {
	va_list		 ap;
	char		*msg;
	file->errors++;
	va_start(ap, fmt);
	if (vasprintf(&msg, fmt, ap) == -1)
		fatalx("yyerror vasprintf");
	va_end(ap);
	logit(LOG_CRIT, "%s:%d: %s", file->name, yylval.lineno, msg);
	free(msg);
	return (0);
}

int kw_cmp(const void *k, const void *e) {
	return (strcmp(k, ((const struct keywords *)e)->k_name));
}

int lookup(char *s) {

	/* this has to be sorted always */
	static const struct keywords keywords[] = {
		{ "baud",		BAUD},
		{ "data",		DATA},
		{ "default",		DEFAULT},
		{ "device",		DEVICE},
		{ "hardware",		HARDWARE},
		{ "listen",		LISTEN},
		{ "location",		LOCATION},
		{ "parity",		PARITY},
		{ "password",		PASSWORD},
		{ "port",		PORT},
		{ "stop",		STOP}
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

unsigned char	*parsebuf;
int		 parseindex;
unsigned char	 pushback_buffer[MAXPUSHBACK];
int		 pushback_index = 0;

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
	unsigned char	 buf[8096];
	unsigned char	*p, *val;
	int		 quotec, next, c;
	int		 token;
	
	top:
	p = buf;
	while ((c = lgetc(0)) == ' ' || c == '\t')
		; /* nothing */
		
		yylval.lineno = file->lineno;
	if (c == '#')
		while ((c = lgetc(0)) != '\n' && c != EOF)
			; /* nothing */
			if (c == '$' && parsebuf == NULL) {
				while (1) {
					if ((c = lgetc(0)) == EOF)
						return (0);
					
					if (p + 1 >= buf + sizeof(buf) - 1) {
						yyerror("string too long");
						return (findeol());
					}
					if (isalnum(c) || c == '_') {
						*p++ = c;
						continue;
					}
					*p = '\0';
					lungetc(c);
					break;
				}
				val = symget(buf);
				if (val == NULL) {
					yyerror("macro '%s' not defined", buf);
					return (findeol());
				}
				parsebuf = val;
				parseindex = 0;
				goto top;
			}
			
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
						err(1, "yylex: strdup");
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
			x != '!' && x != '=' && x != '#' && \
			x != ',' && x != ';' && x != '/'))
			
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
						err(1, "yylex: strdup");
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

int
check_file_secrecy(int fd, const char *fname)
{
	struct stat	st;
	
	if (fstat(fd, &st)) {
		log_warn("cannot stat %s", fname);
		return (-1);
	}
	if (st.st_uid != 0 && st.st_uid != getuid()) {
		log_warnx("%s: owner not root or current user", fname);
		return (-1);
	}
	if (st.st_mode & (S_IWGRP | S_IXGRP | S_IRWXO)) {
		log_warnx("%s: group writable or world read/writable", fname);
		return (-1);
	}
	return (0);
}

struct file *
pushfile(const char *name, int secret)
{
	struct file	*nfile;
	
	if ((nfile = calloc(1, sizeof(struct file))) == NULL) {
		log_warn("%s: malloc", __func__);
		return (NULL);
	}
	if ((nfile->name = strdup(name)) == NULL) {
		log_warn("%s: malloc", __func__);
		free(nfile);
		return (NULL);
	}
	if ((nfile->stream = fopen(nfile->name, "r")) == NULL) {
		log_warn("%s: %s", __func__, nfile->name);
		free(nfile->name);
		free(nfile);
		return (NULL);
	} else if (secret &&
		check_file_secrecy(fileno(nfile->stream), nfile->name)) {
		fclose(nfile->stream);
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
symset(const char *nam, const char *val, int persist)
{
	struct sym	*sym;
	
	for (sym = TAILQ_FIRST(&symhead); sym && strcmp(nam, sym->nam);
	     sym = TAILQ_NEXT(sym, entry))
	     ;	/* nothing */
	     
	     if (sym != NULL) {
		     if (sym->persist == 1)
			     return (0);
		     else {
			     free(sym->nam);
			     free(sym->val);
			     TAILQ_REMOVE(&symhead, sym, entry);
			     free(sym);
		     }
	     }
	     if ((sym = calloc(1, sizeof(*sym))) == NULL)
		     return (-1);
	     
	     sym->nam = strdup(nam);
	     if (sym->nam == NULL) {
		     free(sym);
		     return (-1);
	     }
	     sym->val = strdup(val);
	     if (sym->val == NULL) {
		     free(sym->nam);
		     free(sym);
		     return (-1);
	     }
	     sym->used = 0;
	     sym->persist = persist;
	     TAILQ_INSERT_TAIL(&symhead, sym, entry);
	     return (0);
}

char *
symget(const char *nam)
{
	struct sym	*sym;
	
	TAILQ_FOREACH(sym, &symhead, entry)
	if (strcmp(nam, sym->nam) == 0) {
		sym->used = 1;
		return (sym->val);
	}
	return (NULL);
}

int
parse_config(const char *filename, struct device *xdev)
{
	struct sym	*sym, *next;

	if ((file = pushfile(filename, 0)) == NULL) {
		fatalx("%s: cannot initialize configuration", __func__);
		return (-1);
	}
	topfile = file;

	yyparse();
	errors = file->errors;
	popfile();

	endservent();
	endprotoent();

	if (default_port == 0)
		fatalx("no default port defined");
	
	/* Free macros */
	for (sym = TAILQ_FIRST(&symhead); sym != NULL; sym = next) {
		next = TAILQ_NEXT(sym, entry);
		if (!sym->persist) {
			free(sym->nam);
			free(sym->val);
			TAILQ_REMOVE(&symhead, sym, entry);
			free(sym);
		}
	}
	return (errors ? -1 : 0);
}

struct device *
new_device(char *name)
{
	struct device	*p;
	
	if ((p = calloc(1, sizeof(*p))) == NULL)
		fatalx("no calloc");
	
	if ((p->name = strdup(name)) == NULL)
		fatalx("no name");
	
	return (p);
}
#line 714 "y.tab.c"
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
#line 98 "parse.y"
{ file->errors++; }
break;
case 6:
#line 100 "parse.y"
{
			default_port = yyvsp[0].v.number;
		}
break;
case 9:
#line 107 "parse.y"
{ currentdevice->port = yyvsp[0].v.number; }
break;
case 10:
#line 108 "parse.y"
{
			currentdevice->devicelocation = yyvsp[0].v.string;
		}
break;
case 12:
#line 111 "parse.y"
{ currentdevice->baud = yyvsp[0].v.number; }
break;
case 13:
#line 112 "parse.y"
{ currentdevice->databits = yyvsp[0].v.number; }
break;
case 14:
#line 113 "parse.y"
{ currentdevice->parity = yyvsp[0].v.string; }
break;
case 15:
#line 114 "parse.y"
{ currentdevice->stopbits = yyvsp[0].v.number; }
break;
case 16:
#line 115 "parse.y"
{ currentdevice->hwctrl = yyvsp[0].v.number; }
break;
case 17:
#line 116 "parse.y"
{ currentdevice->password = yyvsp[0].v.string; }
break;
case 18:
#line 118 "parse.y"
{
			currentdevice = new_device(yyvsp[0].v.string);
			currentdevice->name = 		yyvsp[0].v.string;
			currentdevice->port = 		default_port;
			currentdevice->baud = 		DEFAULT_BAUD;
			currentdevice->databits =	0;
			currentdevice->parity =		NULL;
			currentdevice->stopbits =	0;
			currentdevice->hwctrl =		0;
			currentdevice->password =	NULL;
		}
break;
case 19:
#line 128 "parse.y"
{
			TAILQ_INSERT_TAIL(&devices, currentdevice, entry);
			currentdevice = NULL;
		}
break;
#line 973 "y.tab.c"
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
