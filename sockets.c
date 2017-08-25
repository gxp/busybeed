/* $OpenBSD: sockets.c v.1.03 2017/08/25 10:21:00 baseprime Exp $ */
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
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "busybeed.h"

struct sock_conf		*s_socks;
extern struct pollfd		*pfds, *tmppfds;
extern int			 nfds, c_nfds, pfdcnt, clients_start;
extern char			 default_port[6];

int
create_sockets(struct sock_conf *x_socks, struct s_conf *x_devs, char *c_port)
{
	struct s_device			*ldevs;
	struct s_socket			*lsocks;
	char				*cport;
	s_devs =			 x_devs;
	s_socks =			 x_socks;
	char				*iface = NULL;
	int				 sock_r, listener, i, fail, cont,
					     mult_d, defp;
	fail = 0;
	cont = 0;
	mult_d = 1;
	defp = 0;
	cport = c_port;
	if (cport != '\0')
		mult_d = 0;
	if (mult_d)
		TAILQ_INIT(&s_socks->s_sockets);
	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		cont = 0;
		if (mult_d)
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (strcmp(ldevs->port, lsocks->port) == 0) {
					cont = 1;
					break;
				}
			}
		else
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (strcmp(ldevs->port, cport) == 0 &&
				    ldevs->connected == 0) {
					if (strcmp(ldevs->port, default_port)
					    == 0)
						defp = 1;
					break;
				}
			}
		if (cont)
			continue;
		c_socket = new_socket(ldevs->port);
		if (ldevs->bind_interface != '\0')
			iface = get_ifaddrs(ldevs->bind_interface);
		else
			iface = NULL;
		if (strlcpy(c_socket->port, ldevs->port,
			    sizeof(c_socket->port)) == '\0')
			fatalx("port copy failure");
		if (defp == 0) {
			sock_r = c_socket->listener = create_socket(ldevs->port,
			    iface, TCP);
		}
		else
			sock_r = 0;
		if (mult_d)
			s_socks->count++;
		if (sock_r == -1)
			return -1;
		else if (sock_r == -2) {
			fail = 1;
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (strcmp(ldevs->port, lsocks->port) == 0) {
					listener = lsocks->listener;
					if (mult_d)
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
		if (mult_d)
			TAILQ_INSERT_TAIL(&s_socks->s_sockets, c_socket, entry);
		else {
			/* add back to spfds */
			int tots = 0;
			if (defp == 0) {
				clients_start++;
				pfdcnt++;
				tots++;
				nfds++;
			}
			nfds++;
			clients_start++;
			pfdcnt++;
			tots++;
			i = 0;
			if((tmppfds = realloc(pfds, pfdcnt *
			    sizeof(struct pollfd))) == NULL)
				fatal("realloc");
			c_nfds = nfds;
			pfds = tmppfds;
			for (i = pfdcnt-tots-1; i >= 0; i--) {
				pfds[i+tots].fd = pfds[i].fd;
				pfds[i+tots].events = pfds[i].events;
				pfds[i+tots].revents = pfds[i].revents;
			}
			i = 0;
			pfds[i].fd = ldevs->fd;
			pfds[i].events = POLLIN;
			pfds[i++].revents = 0;
			if (defp == 0) {
				pfds[i].fd = listener;
				pfds[i++].events = POLLIN;
			}
			log_info("device %s reconnection attempted",
			    ldevs->name);
			break;
		}
	}
	return 0;
}

int
create_socket(char *port, char *b_iface, int type)
{
	struct addrinfo		 addr_hints, *addr_res, *loop_res;
	int 			 sock_fd, gai, o_val;
	memset(&addr_hints, 0, sizeof(addr_hints));
	addr_hints.ai_family = AF_UNSPEC;
	o_val = 1;
	switch (type) {
	case UDP:
		addr_hints.ai_socktype = SOCK_DGRAM;
		addr_hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
		addr_hints.ai_protocol = 0;
		break;
	case TCP:
	default:
		addr_hints.ai_socktype = SOCK_STREAM;
		addr_hints.ai_flags |= AI_PASSIVE;
		break;
	}
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
	if (type == TCP) {
		if (listen(sock_fd, SOMAXCONN) == -1) {
			fatal("unable to listen on socket");
			return -1;
		}
	}
	return sock_fd;
}

int
open_client_socket(char *ip_addr, int xport)
{
	struct hostent			*server;
	struct sockaddr_in		 servaddr;
	int				 client_fd, cport, nb;
	char				*sockaddr;
	nb = 0;
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
	/*
	 * We set this to non_blocking at first. This stops timeouts from
	 * occurring when we don't see an ip_addr. Device will look
	 * connected. When recv() fails, will then attempt to connect to
	 * ip_addr again. Set back to blocking after connect.
	 */
	nb |= O_NONBLOCK;
	fcntl(client_fd, F_SETFL, nb);
	if (connect(client_fd, (struct sockaddr *)&servaddr,
	    sizeof(servaddr)) == -1)
		log_warn("can't connect ip: %s", sockaddr);
	nb &= ~O_NONBLOCK;
	fcntl(client_fd, F_SETFL, nb);
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
