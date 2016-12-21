/* $OpenBSD: busybee.c v.1.00 2016/11/29 17:42:00 baseprime Exp $ */
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

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "busybeed.h"
#include "json.h"

volatile sig_atomic_t		 bb_quit = 0;
void				 bb_sighdlr(int);

static u_char			 buff[BUFFRSIZE];
int				 max_clients = 1, max_subscriptions = 1;
int				 ph = -1, c_retry = 30;
int				 j, c_nfds, nfds, pfdcnt, clients_start;

struct pollfd			*pfds;

void
bb_sighdlr(int sig)
{
	switch (sig) {
		case SIGINT:
		case SIGTERM:
			bb_quit = 1;
			break;
	}
}

int
packet_handler(struct client_conf *cconf, struct pollfd *x_pfds, u_char *x_buff,
	       int i, int x_rcv)
{
	struct pollfd			*spfds;
	u_char				*s_buff;
	int				 s_rcv, sb;

	s_rcv =				 x_rcv;
	s_buff =			 x_buff;
	spfds =				 x_pfds;

	/*
	 * inspect packet for subscribe
	 * only socket subscribers can pass a subscription packet
	 * see client_subscribe for more information
	 */

	if (s_buff[0] == 0x7E && s_buff[1] == 0x7E && s_buff[2] == 0x7E &&
	    i >= clients_start) {
		sb = client_subscribe(cconf, spfds[i].fd, s_buff);

		if (sb == -1) {
			log_warnx("bad subscribe packet");
			//close(spfds[i].fd);
			//clean_pfds(spfds, i);
		}
	} else {
		/* forward packet to subscribers */
		printf("NO subscribe, pass packet\n");
		/*printf("Bytes: %i\n", s_rcv);
		printf("Data: %s\n\n", s_buff);*/

	}
	return 0;
}

void
clean_pfds(struct pollfd *x_pfds, int i)
{
	struct pollfd			*spfds;
	spfds =				 x_pfds;
	for(j = i; j < c_nfds; j++)
	{
		spfds[j].fd =
		spfds[j+1].fd;
	}
	nfds--;
	if (i >= clients_start) {
		log_info("client connection closed");
	} else {
		/* could do reconnection
		 * here for ip_addr
		 * timeouts? maybe later
		 * really not needed by
		 * me now unless 8266
		 * has a timeout period
		 * will need to test
		 * later
		 * 
		 * start timer, need to add a retry period to parse
		 */
		pfdcnt--;
		clients_start--;
		log_info("non-client connection closed");
		spfds = realloc(spfds, pfdcnt* sizeof(struct pollfd));
		if (spfds == '\0') {
			fatal("realloc");
		}
	}
}

pid_t
busybee_main(int pipe_prnt[2], int fd_ctl, struct busybeed_conf *xconf,
	     struct s_conf * x_devices, struct sock_conf *x_socks,
	     struct client_conf *x_clients)
{
	pid_t				 pid;

	struct s_device			*ldevs;
	struct s_socket			*lsocks;

	struct busybeed_conf	 	*bconf;
	struct s_conf			*sdevs;
	struct sock_conf		*socks;
	struct client_conf		*sclients;

	bconf =				 xconf;
	sdevs =				 x_devices;
	socks =				 x_socks;
	sclients =			 x_clients;

	int				 pi = 0, i, pollsocks;
	int				 rcv, c_conn = 0;
	int				 n_client = -1, is_client = 0;

	clients_start =			 (sdevs->count + socks->count);
	nfds =				 clients_start;
	pfdcnt =			 (sdevs->count + socks->count +
					  max_clients);
	pfds =				 malloc(pfdcnt*sizeof(struct pollfd));

	/* load up fds */
	TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
		pfds[pi].fd =		 lsocks->listener;
		pfds[pi++].events =	 POLLIN;
	}
	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		pfds[pi].fd =		 ldevs->fd;
		pfds[pi++].events =	 POLLIN;
	}

	memset(sclients, 0, sizeof(sclients));
	TAILQ_INIT(&sclients->clients);

	switch (pid = fork()) {
		case -1:
			fatal("cannot fork");
			break;
		case 0:
			break;
		default:
			return (pid);
	}

	if (!xconf->debug) {
		log_init(xconf->debug, LOG_DAEMON);
		if (setsid() == -1)
			fatal("setsid");
	}
	log_procinit("busybee");

	if (pledge("stdio tty rpath wpath inet proc",
		NULL) == -1)
		err(1, "pledge");

	signal(SIGTERM, bb_sighdlr);
	signal(SIGINT, bb_sighdlr);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_DFL);

	while (bb_quit == 0) {
		/* start polling */
		pollsocks = poll(pfds, nfds, -1);
		if (pollsocks == -1) {
			fatal("poll() failed");
		}

		c_nfds = nfds;

		for (i = 0; i < c_nfds; i++) {
			if(pfds[i].revents == 0)
				continue;

			/* only check sockets for client connections */
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (pfds[i].fd == lsocks->listener) {
					is_client = 1;
					break;
				}
			}

			if (is_client == 1) {
				do {
					n_client = accept(lsocks->listener,
								NULL, NULL);
					if (n_client == -1) {
						if (errno != EWOULDBLOCK) {
							fatal("failed");
							break;
						}
					}
					if (nfds < pfdcnt) {
						int opts =
						    fcntl(n_client, F_GETFL);
						if(opts < 0)
						{
							fatal("opts failed");
						}

						opts |= O_NONBLOCK;

						if(fcntl(n_client,
							F_SETFL, opts) < 0)
						{
							fatal("F_SETFL");
						}
						pfds[nfds].fd = n_client;
						pfds[nfds].events = POLLIN;
						log_info("client accepted");
						nfds++;
					} else {
						log_info(
						    "max_clients exhausted");
						close(n_client);
					}
				} while (n_client == -1);
				is_client = 0;
			} else {
				c_conn = 0;
				do {
					memset(buff, 0, sizeof(buff));
					rcv = read(pfds[i].fd, buff,
						  sizeof(buff));
					if (rcv < 0) {
						if (errno != EWOULDBLOCK)
						{
							log_warn("recv()"	\
								 " failed");
						}
						if (errno == EBADF) {
							fatal("error");
						}
						break;
					}
					if (rcv == 0) {
						close(pfds[i].fd);
						clean_pfds(pfds, i);
						break;
					}

					ph = packet_handler(sclients, pfds,
							    buff, i, rcv);

					if (ph == 0)
						break;

				} while (bb_quit == 0);
			}
		}
		n_client = -1;
	}
	for (i = 0; i < nfds; i++)
	{
		if(pfds[i].fd >= 0)
			close(pfds[i].fd);
	}
	_exit(0);
}
