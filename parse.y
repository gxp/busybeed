/* $OpenBSD: parse.y,v 1.19 2016/06/27 15:41:17 tedu Exp $ */
/*
 * Copyright (c) 2015 Ted Unangst <tedu@openbsd.org>
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





/* Tracey temp notes

yytext variable holds the raw data read by lex during parsing
yylval is used to exchange the actual value between the two systems


token increased precidence from top to bottom!

http://www.ibm.com/developerworks/aix/tutorials/au-lexyacc/
https://luv.asn.au/overheads/lex_yacc/yacc.html








set two globals
	max devices
	default port

then device setups

device "name"
	listen on port *optional*

	location "fd"

		baud
		data *optional*
		parity *optional*
		stop *optional*
		hardware *optional*
		password *optional*


*/

/* c declarations */
%{
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

#include "busybee.h"

int 		yyparse(void);
int 		yylex(void);
int		yyerror(const char *, ...);

%}

/* yacc declarations */

%token BAUD DATA PARITY STOP PASSWORD
%token LISTEN LOCATION
%token DEVICES
%left MAX DEFAULT DEVICE

/* grammar rules */
%%

grammar		:/* empty */
		| grammar '\n'
;







%%

/* additional c code */
struct device {
	char*		devicename;
	int		port;
	char*		devicelocation;
	int		baud;
	int		databits;
	char*		parity; /* none, even, odd, space */
	int		stopbits;
	int		hardwarecontrol;
	char*		password;	
}

struct device devices[];

int yyerror(const char *s) {
	va_list		ap;
	char		*msg;

	file->errors++;
	va_start(ap, s);
	if (vasprintf(&msg, s, ap) == -1_
		fatalx("yyerror vasprintf");
	va_end(ap);
	logit(LOG_CRIT, "%s:%d: %s"

	fprintf (stderr, "%s\n", s);
	return 0;
}

int yylex(void) {

}
