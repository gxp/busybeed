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

#include <arpa/inet.h>
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

int
create_sockets(struct sock_conf *x_socks, struct s_conf *x_devs)
{
	struct s_device			*ldevs;
	s_devs =			 x_devs;
	s_socks =			 x_socks;
	char				*iface = NULL;

	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		c_socket = new_socket(ldevs->port);

		if (ldevs->bind_interface != '\0')
			iface = get_ifaddrs(ldevs->bind_interface);

		if ((c_socket->listener = create_socket(ldevs->port,
			iface)) == -1)
			return -1;
	}
	return 0;
}

int
create_socket(char *port, char *b_iface)
{
	int 			 sock_fd;
	int gai, o_val = 1;
	struct addrinfo addr_hints, *addr_res, *loop_res;
	
	memset(&addr_hints, 0, sizeof(addr_hints));
	/* accept any family, use streams, and make passive */
	addr_hints.ai_family = AF_UNSPEC;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_flags |= AI_PASSIVE;

	if((gai = getaddrinfo(b_iface, port, &addr_hints, &addr_res)) != 0) {
		fatalx("getaddrinfo failed: %s", gai_strerror(gai));
		return -1;
	}

	for(loop_res = addr_res; loop_res != NULL;
		loop_res = loop_res->ai_next) {
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

		if(bind(sock_fd, loop_res->ai_addr,
			loop_res->ai_addrlen) == -1) {
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

	if(listen(sock_fd, SOMAXCONN) == -1) {
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
	
	if (strlcpy(sock->port, strdup(port), sizeof(sock->port)) == '\0')
		fatalx("no s_sock port");

	return (sock);
};

char
*get_ifaddrs(char *name)
{
	struct ifaddrs *ifap, *ifa;
	char *addr;
	
	if (getifaddrs(&ifa) == -1)
		fatalx("getifaddrs error");

	ifap = ifa;

	while (ifap) {
		if ((ifap->ifa_addr) &&
			((ifap->ifa_addr->sa_family == AF_INET) ||
			(ifap->ifa_addr->sa_family == AF_INET6))) {

			if (ifap->ifa_addr->sa_family == AF_INET) {
				struct sockaddr_in *in =
				(struct sockaddr_in*) ifap->ifa_addr;
				addr = inet_ntoa(in->sin_addr);
			} else {
				getnameinfo(ifap->ifa_addr,
					sizeof(struct sockaddr_in6), addr,
					sizeof(addr), NULL, 0, NI_NUMERICHOST);
			}

			if (strcmp(name, ifap->ifa_name) == 0) {
				freeifaddrs(ifap);
				return addr;
			}
		}
		ifap = ifap->ifa_next;
	}

	freeifaddrs(ifap);
	return NULL;
}