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
		
		/* client will need this info later. keep here for now
		 * printf("Name: %s\n", ldevs->name);
		printf("Port: %i\n", ldevs->port);
		printf("Location: %s\n", ldevs->location);
		printf("Password: %s\n", ldevs->password); */
		
		c_socket = new_socket(ldevs->port);
		// add listnener
	}
	
	
	
	
	
	return 0;
}

struct s_socket *
new_socket(int port)
{
	struct s_socket	*sock;
	
	if ((sock = calloc(1, sizeof(*sock))) == NULL)
		fatalx("no s_sock calloc");
	
	if (port < 1)
		fatalx("no s_sock port");
	
	return (sock);
};