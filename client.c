/* $OpenBSD: client.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
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

#include <sys/queue.h>

#include <errno.h>
#include <pthread.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "busybeed.h"

extern int				clients_start, c_nfds, nfds;

int
client_subscribe(struct client_conf *cconf, int pfd, unsigned char *x_buff)
{
	unsigned char			*s_buff;
	int				 parsedb = 0;
	struct client_conf		*xcconf;
	/*
	 * A human readable packet is used for subscribing. The format is as
	 * follows, without the line breaks and tabs. This is for initial
	 * readability! Subscription packets must begin with 3 * 0x7E.
	 * 
	 * ~~~subscribe{
	 * 	{name,"CLIENTNAME"},
	 * 	{devices{
	 * 		device{"dev1","password1"},
	 * 		device{"dev2","password2"}
	 * 		}
	 * 	}
	 * }
	 * 
	 * Accurate example:
	 * 
	 * ~~~subscribe{{name,"CLIENTNAME"},{devices{device{"dev1","password1"},device{"dev2","password2"}}}}
	 * ~~~subscribe{{name,"ttest"},{devices{device{"data_xbee","Pass1234}}}}
	 * If your packet is not accurate, it will fail.
	 */

	s_buff =			 x_buff;
	xcconf =			 cconf;

	memmove(s_buff, s_buff+3, strlen(s_buff+3)+1);

	parsedb = parse_buffer(xcconf, s_buff, pfd);

	return parsedb;
}

void
*run_client_timer(void *data)
{
	struct pollfd			*spfds;
	struct client			*sclient;
	struct client_conf		*sclients;
	struct client_timer_data	*cdata;

	int				 seconds, c_pfd;
	pthread_t			 me;

	void (*t_fptr)(struct pollfd *, struct client_conf *);

	me =				 pthread_self();
	cdata =				 data;
	spfds =				 cdata->pfd;
	c_pfd =				 cdata->c_pfd;
	sclients =				 cdata->cconf;
	seconds =			 cdata->seconds;
	t_fptr = 			 cdata->fptr;

	TAILQ_FOREACH(sclient, &sclients->clients, entry) {
		if (sclient->pfd == c_pfd) {
			sclient->me_thread = me;
			break;
		}
	}

	sleep(seconds);

	(void) (*t_fptr)(spfds, sclients);
	pthread_exit(NULL);
	return NULL;
}

void
start_client_timer(struct client_timer_data *cdata)
{
	struct client_timer_data	*cldata;
	pthread_t			 client_check;
	int				 tcheck;

	cldata =			 cdata;

	tcheck = pthread_create(&client_check, NULL, run_client_timer,
				(void *) cldata);
	if (tcheck)
		fatalx("thread creation failed");
}

void
test_client(struct pollfd *x_pfds, struct client_conf *cconf)
{
	struct pollfd			*spfds;
	struct client			*sclient;
	struct client_conf		*sclients;
	int				 i;
	pthread_t			 me;
	
	spfds =				 x_pfds;
	sclients =			 cconf;
	i =				 clients_start;
	c_nfds =			 nfds;
	me =				 pthread_self();

	TAILQ_FOREACH(sclient, &sclients->clients, entry) {
		if (sclient->subscribed != 1) {
			for (i = 0; i < c_nfds; i++) {
				if ((spfds[i].fd == sclient->pfd) &&
					(sclient->me_thread == me)) {
					clean_pfds(sclients, spfds, i, NULL);
					break;
				}
			}
		}
	}
	pthread_exit(NULL);
}

void
do_subscribe(int mypfd, int devfd, struct client_conf *cconf)
{
	struct client_conf		*sclients;
	struct client			*sclient;

	sclients =			 cconf;

	TAILQ_FOREACH(sclient, &sclients->clients, entry) {
		if (sclient->pfd == mypfd) {
			log_info("subscribing %s", sclient->name);
			sclient->subscribed = 1;
			sclient->subscriptions[sclient->lastelement] = devfd;
			sclient->lastelement++;
			break;
		}
	}
}

struct client *
new_client(int pfd)
{
	struct client	*client;
	if ((client = calloc(1, sizeof(*client) + (max_subscriptions *
		sizeof(int)))) == NULL)
		fatalx("no client calloc");
	
	if ((client->pfd = pfd) < 1)
		fatalx("no client pfd");
	
	return (client);
};