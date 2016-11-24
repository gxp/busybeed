# $OpenBSD: Makefile,v 1.73 2016/11/19 09:17:22 baseprime Exp $

PROG=		busybeed
SRCS=		parse.y busybeed.c client.c log.c serial.c sockets.c

DEBUG=		-g -DDEBUG=3 -O0
CFLAGS+=	-Wall -I${.CURDIR}
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith
CFLAGS+=	-Wsign-compare
CLEANFILES+=	y.tab.h
MAN=		busybeed.8

.include <bsd.prog.mk>
