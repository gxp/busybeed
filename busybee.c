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

volatile sig_atomic_t		 bb_quit = 0;
void				 bb_sighdlr(int);

static unsigned char		 buff[BUFFRSIZE];

int				 max_clients = 1, max_subscriptions = 1;

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

pid_t
busybee_main(int pipe_prnt[2], int fd_ctl, struct busybeed_conf *xconf,
	     struct s_conf * x_devices, struct sock_conf *x_socks)
{
	pid_t				 pid;

	struct s_device			*ldevs;
	struct s_socket			*lsocks;

	struct busybeed_conf	 	*bconf;
	struct s_conf			*sdevs;
	struct sock_conf		*socks;

	bconf =				 xconf;
	sdevs =				 x_devices;
	socks =				 x_socks;

	int 				 clients_start = (sdevs->count +
						socks->count);
	int				 pfdcnt = (sdevs->count + socks->count +
						max_clients);
	int				 pi = 0, i, j, nfds, pollsocks, c_nfds;
	int				 rcv, c_conn = 0;
	int				 n_client = -1, is_client = 0;

	struct pollfd			 pfds[pfdcnt];

	/* load up fds */
	TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
		pfds[pi].fd =		 lsocks->listener;
		pfds[pi++].events =	 POLLIN;
	}
	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		pfds[pi].fd =		 ldevs->fd;
		pfds[pi++].events =	 POLLIN;
	}
	nfds = pi;

	switch (pid = fork()) {
		case -1:
			fatal("cannot fork");
			break;
		case 0:
			break;
		default:
			return (pid);
	}

	if (pledge("stdio tty rpath wpath inet proc",
		NULL) == -1)
		err(1, "pledge");

	log_procinit("busybeed");

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

		for (i = 0; i < c_nfds; i++)
		{
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
						if (errno !=
							EWOULDBLOCK) {
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
					if (rcv < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							log_info(
							       "recv() failed");
						}
						break;
					}
					if (rcv == 0)
					{
						pfds[i].fd = 0;
						close(pfds[i].fd);
						for(j = i; j < nfds; j++)
						{
							pfds[j].fd =
								   pfds[j+1].fd;
						}
						nfds--;
						log_info("connection closed");
						break;
					}
					/* inspect packet for subscribe */
					if (buff[0] == 0x7E) {
						printf("Subscribe here\n");
						printf("Bytes: %i\n", rcv);
						printf("Data: %s\n\n", buff);
						break;
					} else {
					/* forward packet to subscription */
						printf("NO subscribe\n");
						printf("Bytes: %i\n", rcv);
						printf("Data: %x,%x\n\n", buff[0], buff[1]);
						break;
					}
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
