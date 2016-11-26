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

	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		c_socket = new_socket(ldevs->port);
		if ((c_socket->listener = create_socket(ldevs->port)) == -1)
			return -1;
	}

	return 0;
}

int
create_socket(char *port)
{
	int sock_fd;
	struct addrinfo ahints, *servinfo, *i;

	int gai, o_val = 1;
	
	memset(&ahints, 0, sizeof(ahints));
	ahints.ai_family = AF_UNSPEC;
	ahints.ai_socktype = SOCK_STREAM;
	ahints.ai_flags |= AI_PASSIVE;
	
	if((gai = getaddrinfo(NULL, port, &ahints, &servinfo)) != 0)
	{
		fatalx("getaddrinfo failed");
		fatalx(gai_strerror(gai));
		return -1;
	}

	for(i = servinfo; i != NULL; i = i->ai_next)
	{
		if((sock_fd = socket(i->ai_family, i->ai_socktype,
			i->ai_protocol)) == -1)
			continue;

		if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &o_val,
			sizeof(int)) == -1)
		{
			fatalx("setsockopt error");
			freeaddrinfo(servinfo);
			return -1;
		}

		if(bind(sock_fd, i->ai_addr, i->ai_addrlen) == -1)
		{
			close(sock_fd);
			continue;
		}
		break;
	}

	if(i == NULL)
	{
		fatalx("can't bind to port");
		freeaddrinfo(servinfo);
		return -1;
	}

	freeaddrinfo(servinfo);

	if(listen(sock_fd, max_clients) == -1)
	{
		fatalx("socket error");
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