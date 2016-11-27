/* $OpenBSD: sockets.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
/*
 * Copyright (c) 2016 Tracey Emery <tracey@traceyemery.com>
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

#include <netinet/in.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>


#include "busybeed.h"

struct sock_conf		*s_socks;

/*
struct address	*host_v4(const char *);
struct address	*host_v6(const char *);
int		 host_dns(const char *, struct addresslist *,
		    int, struct portrange *, const char *, int);
int		 host_if(const char *, struct addresslist *,
		    int, struct portrange *, const char *, int);
int		 host(const char *, struct addresslist *,
		    int, struct portrange *, const char *, int);
void		 host_free(struct addresslist *);
struct portrange {
	in_port_t		 val[2];
	uint8_t			 op;
};*/

int
create_sockets(struct sock_conf *x_socks, struct s_conf *x_devs)
{
	struct s_device			*ldevs;
	s_devs =			 x_devs;
	s_socks =			 x_socks;

	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		c_socket = new_socket(ldevs->port);
		if ((c_socket->listener = create_socket(ldevs->port,
			ldevs->bind_interface)) == -1)
			return -1;
	}

	return 0;
}

/* interface selection in getaddrinfo */
int
create_socket(char *port, char *b_iface)
{
	int 			 sock_fd;
	char			*iface = NULL;
	struct addrinfo addr_hints, *addr_res, *loop_res;

	int gai, o_val = 1;

	memset(&addr_hints, 0, sizeof(addr_hints));
	/* accept any family, use streams, and make passive */
	addr_hints.ai_family = AF_UNSPEC;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_flags |= AI_PASSIVE;
	/* do interface stuff here */
	/* getifaddrs */
	if (b_iface != '\0') {
		iface = b_iface;
	} else {
		if ((strcmp(bind_interface, "0")) == 0) {
			iface = NULL;
		} else {
			
		}
	}
	
	/* get addrs */
	if((gai = getaddrinfo(iface, port, &addr_hints, &addr_res)) != 0) {
		fatalx("getaddrinfo failed: %s", gai_strerror(gai));
		return -1;
	}




	/* bind loop*/
	for(loop_res = addr_res; loop_res != NULL; loop_res = loop_res->ai_next) {
		if((sock_fd = socket(loop_res->ai_family, loop_res->ai_socktype,
			loop_res->ai_protocol)) == -1) {
			fatalx("unable to create socket");
			return -1;
		}

		if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &o_val,
			sizeof(int)) == -1) {
			fatalx("setsockopt error");
			freeaddrinfo(addr_res);
			return -1;
		}

		if(bind(sock_fd, loop_res->ai_addr, loop_res->ai_addrlen) == -1) {
			close(sock_fd);
			fatalx("unable to bind address");
			return -1;
		}
		break;
	}

	if(loop_res == NULL) {
		fatalx("can't bind to port");
		freeaddrinfo(addr_res);
		return -1;
	}

	freeaddrinfo(addr_res);
	/* error if MAX_REQS reached */
	if(listen(sock_fd, MAX_REQS) == -1) {
		fatalx("unable to listen on socket");
		return -1;
	}

	return sock_fd;
}

struct s_socket *
new_socket(char *port)
{
	struct s_socket	*sock;

	if ((sock = calloc(1, sizeof(*sock))) == NULL)
		fatalx("no s_sock calloc");
	
	if ((sock->port = strdup(port)) == NULL)
		fatalx("no s_sock port");

	return (sock);
};
