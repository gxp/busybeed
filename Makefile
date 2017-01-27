# $OpenBSD: Makefile,v 1.73 2017/01/26 18:17:22 baseprime Exp $

PROG=		busybeed
SRCS=		parse.y busybeed.c client.c log.c serial.c sockets.c
SRCS+=		busybee.c control.c

DEBUG=		-g -DDEBUG=3 -O0

LDFLAGS=	-pthread -lpthread

CFLAGS+=	-Wall -I${.CURDIR}
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith
CFLAGS+=	-Wsign-compare

LDADD+=		-lutil

CLEANFILES+=	y.tab.h
LINKS=		${BINDIR}/busybeed ${BINDIR}/busybctl

MAN=		busybeed.8 busybctl.8 busybeed.conf.5

.include <bsd.prog.mk>
