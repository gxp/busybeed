/* $OpenBSD: sockets.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
/*
 * Copyright (c) 2016 Tracey Emery <tracey@traceyemery.net>
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

/*
 * Someday, add ipv6 to this. Who cares right now. What a hassle.
 * Needed in:
 * 	get_ifaddrs
 * 	create_socket
 * 	open_client_socket
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
	struct s_socket			*lsocks;
	s_devs =			 x_devs;
	s_socks =			 x_socks;
	char				*iface = NULL;
	int				 sock_r, listener;
	int				 fail = 0;

	TAILQ_INIT(&s_socks->s_sockets);

	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		c_socket = new_socket(ldevs->port);

		if (ldevs->bind_interface != '\0')
			iface = get_ifaddrs(ldevs->bind_interface);
		else
			iface = NULL;

		if (strlcpy(c_socket->port, ldevs->port,
			    sizeof(c_socket->port)) == '\0')
			fatalx("port copy failure");

		sock_r = c_socket->listener = create_socket(ldevs->port, iface);
		s_socks->count++;
		if ( sock_r == -1)
			return -1;
		else if (sock_r == -2) {
			fail = 1;
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (strcmp(ldevs->port, lsocks->port) == 0) {
					listener = lsocks->listener;
					s_socks->count--;
					fail = 0;
					break;
				}
			}
			if (fail == 1) {
				fatalx("can't find listener");
				return -1;
			}
		} else {
			listener =  c_socket->listener;
			ldevs->listener = listener;
		}

		//ldevs->listener = listener;
		TAILQ_INSERT_TAIL(&s_socks->s_sockets, c_socket, entry);
	}
	return 0;
}

int
create_socket(char *port, char *b_iface)
{
	int 			 sock_fd;
	int			 gai, o_val = 1;
	struct addrinfo addr_hints, *addr_res, *loop_res;

	memset(&addr_hints, 0, sizeof(addr_hints));
	/* accept any family, use streams, and make passive */
	addr_hints.ai_family = AF_UNSPEC;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_flags |= AI_PASSIVE;

	if ((gai = getaddrinfo(b_iface, port, &addr_hints, &addr_res)) != 0) {
		fatalx("getaddrinfo failed: %s", gai_strerror(gai));
		return -1;
	}

	for(loop_res = addr_res; loop_res != NULL;
		loop_res = loop_res->ai_next) {

		if ((sock_fd = socket(loop_res->ai_family,
		    loop_res->ai_socktype, loop_res->ai_protocol)) == -1) {
			fatalx("unable to create socket");
			return -1;
		}

		if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &o_val,
		    sizeof(int)) == -1) {
			fatalx("setsockopt error");
			freeaddrinfo(addr_res);
			return -1; 
		}

		if (bind(sock_fd, loop_res->ai_addr,
		    loop_res->ai_addrlen) == -1) {
			close(sock_fd);
			log_warnx("%s%s", "bind address busy\n",
			    " -checking existing sockets");
			return -2;
		}

		break;
	}

	if (loop_res == NULL) {
		fatalx("can't bind to port");
		freeaddrinfo(addr_res);
		return -1;
	}

	freeaddrinfo(addr_res);

	if (listen(sock_fd, SOMAXCONN) == -1) {
		fatalx("unable to listen on socket");
		return -1;
	}

	return sock_fd;
}

int
open_client_socket(char *ip_addr, int xport)
{
	int				 client_fd, cport;
	char				*sockaddr;
	
	struct hostent			*server;
	struct sockaddr_in		 servaddr;
	
	sockaddr = ip_addr;
	cport = xport;
	client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (client_fd == -1) {
		log_warnx("failed to open sock_stream");
		return(-1);
	}
	server = gethostbyname(ip_addr);

	if (server == NULL) {
		log_warn("no such host");
		return(-1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memcpy(&servaddr.sin_addr.s_addr, server->h_addr, server->h_length);

	servaddr.sin_family =		 AF_INET;
	servaddr.sin_port =		 htons(cport);

	inet_pton(AF_INET, sockaddr, &(servaddr.sin_addr));

	if (connect(client_fd, (struct sockaddr *)&servaddr,
		sizeof(servaddr)) == -1)
		fatalx("can't connect ip: %s", sockaddr);

	return client_fd;
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
	struct ifaddrs			*ifap, *ifa;
	char				*addr;

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
				/*struct sockaddr_in6 *in6 =
				(struct sockaddr_in6*) ifap->ifa_addr;*/
			}

			if ((strcmp(name, ifap->ifa_name) == 0) &&
			    addr != '\0') {
				return addr;
				freeifaddrs(ifap);
			}
		}
		ifap = ifap->ifa_next;
	}

	freeifaddrs(ifap);
	return NULL;
}