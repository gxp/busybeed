# $OpenBSD: Makefile,v 1.73 2016/11/19 09:17:22 baseprime Exp $

PROG=		busybee
SRCS=		parse.y
SRCS+=		busybee.c log.c serial.c sockets.c

LDADD=		-levent -ltls -lssl -lcrypto -lutil
DPADD=		${LIBEVENT} ${LIBTLS} ${LIBSSL} ${LIBCRYPTO} ${LIBUTIL}
#DEBUG=		-g -DDEBUG=3 -O0
CFLAGS+=	-Wall -I${.CURDIR}
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith
CFLAGS+=	-Wsign-compare
CLEANFILES+=	y.tab.h

.include <bsd.prog.mk>
