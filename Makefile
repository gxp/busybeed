# $OpenBSD: Makefile,v 1.73 2016/11/19 09:17:22 baseprime Exp $

LIBJSON_DIR=	/usr/local
LIBJSON_INCDIR=	$(LIBJSON_DIR)/include
LIBJSON_LIBDIR=	$(LIBJSON_DIR)/lib
PROG=		busybeed
SRCS=		parse.y busybeed.c client.c log.c serial.c sockets.c
SRCS+=		busybee.c control.c

DEBUG=		-g -DDEBUG=3 -O0

#LDFLAGS +=	-L$(LIBJSON_LIBDIR) -ljson-c

#CFLAGS +=	-I$(LIBJSON_INCDIR)/json-c
CFLAGS+=	-Wall -I${.CURDIR}
CFLAGS+=	-Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=	-Wmissing-declarations
CFLAGS+=	-Wshadow -Wpointer-arith
CFLAGS+=	-Wsign-compare

CLEANFILES+=	y.tab.h
#LINKS=		${BINDIR}/busybeed ${BINDIR}/busybctl
LINKS=		busybeed busybctl
MAN=		busybeed.8 busybctl.8 busybeed.conf.5

.include <bsd.prog.mk>