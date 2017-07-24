/* $OpenBSD: busybee.c v.1.01 2016/11/29 17:42:00 baseprime Exp $ */
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

#include <netinet/in.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <imsg.h>
#include <math.h>
#include <pthread.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "busybeed.h"

volatile sig_atomic_t		 bb_quit = 0;
void				 bb_sighdlr(int);

static u_char			 buff[BUFFRSIZE];
int				 max_clients = 1, max_subscriptions = 1;
int				 ph = -1;
int				 j, c_nfds, nfds, pfdcnt, clients_start,
				     wdcheck;
socklen_t			*addrlen;
struct sockaddr			*addr;
struct imsgbuf			*ibuf_main;
struct ctl_conns		 ctl_conns;
extern int			 c_retry;
extern char			 default_port[6];
struct pollfd			*pfds, *tmppfds;

void
bb_sighdlr(int sig)
{
	switch (sig) {
	case SIGHUP:
	case SIGINT:
	case SIGTERM:
		bb_quit = 1;
		break;
	}
}

void
write_packet(int blen, int wfd, char *name, u_char *x_buff,
	struct s_conf *x_devices)
{
	int				 b_len, w_fd, write_it, close_it;
	u_char				*s_buff;
	char				*d_name;
	struct s_conf			*sdevs;
	struct s_device			*ldevs;

	b_len =				 blen;
	w_fd =				 wfd;
	s_buff =			 x_buff;
	d_name =			 name;
	sdevs =				 x_devices;
	write_it =			 1;
	close_it =			 0;

	if (w_fd == -1) {
		TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
			if (strcmp(ldevs->name, d_name) == 0) {
				if ((w_fd = open_client_socket(ldevs->ipaddr,
				    ldevs->cport)) == -1) {
					log_info("socket init failure");
					write_it = 0;
				}
				close_it = 1;
			}
		}
	}

	if (write_it == 1)
		write(w_fd, s_buff, b_len);

	if (close_it == 1)
		shutdown(w_fd,2);
}

int
packet_handler(struct client_conf *cconf, struct pollfd *x_pfds, u_char *x_buff,
	int i, int x_rcv, struct s_conf *x_devices)
{
	struct pollfd			*spfds;
	struct s_conf			*sdevs;
	struct client			*sclient;
	struct client_conf		*sclients;

	sdevs =				 x_devices;
	u_char				*s_buff;
	int				 s_rcv, sb, k, cnfds;

	s_rcv =				 x_rcv;
	s_buff =			 x_buff;
	spfds =				 x_pfds;
	sclients =			 cconf;
	cnfds =				 nfds;

	/*
	 * inspect packet for subscribe
	 * only socket clients can pass a subscription packet
	 * see client_subscribe for more information
	 */

	if (s_buff[0] == 0x7E && s_buff[1] == 0x7E && s_buff[2] == 0x7E &&
	    i >= clients_start) {
		sb = client_subscribe(sclients, spfds[i].fd, s_buff);
		if (sb == -1) {
			log_warnx("bad subscribe packet");
			clean_pfds(sclients, spfds, i, sdevs);
		}
	} else {
		/* forward packet to and from subscribers */
		if (i < clients_start) {
			/* dev packet */
			TAILQ_FOREACH(sclient, &sclients->clients, entry) {
				for (k = 0; k < cnfds; k++) {
					if (sclient->subscriptions[k] ==
					    spfds[i].fd)
						write_packet(s_rcv,
						    sclient->pfd, NULL, s_buff,
						    sdevs);
				}
			}
		} else {
			/* client packet */
			TAILQ_FOREACH(sclient, &sclients->clients, entry) {
				if (sclient->pfd == spfds[i].fd &&
				    sclient->subscribed == 1)
					for (k = 0; k < max_subscriptions; k++)
					{
						if (sclient->subscriptions[k]
						    != 0)
							write_packet(s_rcv,
							  sclient->
							  subscriptions[k],
							  sclient->
							  subscriptions_name[k],
							  s_buff, sdevs);
					}
			}
		}
	}
	return 0;
}

void
clean_devs(int subscriptions[max_subscriptions], struct s_conf *x_devices)
{
	struct s_conf			*sdevs;
	struct s_device			*ldevs;
	int				 subs;
	sdevs =				 x_devices;

	for (subs = 0; subs < max_subscriptions; subs++) {
		TAILQ_FOREACH(ldevs, &sdevs->s_devices, entry) {
			if (ldevs->fd == subscriptions[subs])
				ldevs->subscribers--;
		}
	}
}

void
clean_pfds(struct client_conf *cconf, struct pollfd *x_pfds, int i,
	   struct s_conf *x_devices)
{
	struct pollfd			*spfds;
	struct client			*sclient, *tmp_sclient;
	struct client_conf		*sclients;
	struct s_conf			*sdevs;
	struct s_device			*ldevs;

	int				 toclose, subs, subcnt;
	int				 client_closed = 0;
	sdevs =				 x_devices;
	spfds =				 x_pfds;
	toclose =			 spfds[i].fd;
	sclients =			 cconf;

	TAILQ_FOREACH_SAFE(sclient, &sclients->clients, entry, tmp_sclient) {
		tmp_sclient = TAILQ_NEXT(sclient, entry);
		if (sclient->pfd == spfds[i].fd) {
			/*
			 * clean subscriptions
			 * if x_devices == null, it's coming from test_client()
			 * and no subscriptions have been attempted, so skip
			 */
			if (sdevs != NULL) {
				clean_devs(sclient->subscriptions, sdevs);
				log_info("client %s closing", sclient->name);
			}
			TAILQ_REMOVE(&sclients->clients, sclient, entry);
			client_closed = 1;
			break;
		}
	}
	nfds--;
	c_nfds = nfds;
	close(toclose);

	/* shift fds */
	for (j = i; j < c_nfds; j++) {
		spfds[j].fd = spfds[j+1].fd;
		spfds[j].events = spfds[j+1].events;
		spfds[j].revents = spfds[j+1].revents;
	}
	for (j = c_nfds; j < pfdcnt; j++) {
		spfds[j].fd = 0;
	}

	if (client_closed)
		log_info("client connection closed");
	else {
		/*
		 * shutdown port for device
		 */
		char *tmp_port;
		int really_close = 1;
		TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
			if (toclose == ldevs->fd) {
				tmp_port = ldevs->port;
				ldevs->connected = 0;
				break;
			}
		}
		TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
			if (toclose != ldevs->fd && strcmp(ldevs->port,
				tmp_port) == 0) {
				really_close = 0;
				break;
			}
		}
		TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
			if (toclose == ldevs->fd && strcmp(ldevs->port,
			    default_port) != 0 && really_close) {
				for (j = 0; j < c_nfds; j++) {
					if (spfds[j].fd == ldevs->listener) {
						shutdown(ldevs->listener, 2);
						close(ldevs->listener);
						nfds--;
						c_nfds = nfds;
						pfdcnt--;
						clients_start--;
						ldevs->subscribers = 0;
						for (i = j; i < c_nfds; i++) {
							spfds[i].fd =
							    spfds[i+1].fd;
							spfds[i].events =
							    spfds[i+1].events;
							spfds[i].revents =
							    spfds[i+1].revents;
						}
						for (j = i; j < pfdcnt; j++) {
							spfds[j].fd = 0;
						}
					}
					break;
				}
			}
		}
		pfdcnt--;
		clients_start--;
		log_info("non-client connection closed");

		/* clean clients */
		TAILQ_FOREACH(sclient, &sclients->clients, entry) {
			subcnt = 0;
			for (subs = 0; subs < max_subscriptions; subs++) {
				if (toclose == sclient->subscriptions[subs]) {
					sclient->subscriptions[subs] = 0;
				}
				if (sclient->subscriptions[subs] == 0)
					subcnt++;
			}
			if (subcnt == max_subscriptions) {
				for (i = clients_start; i < pfdcnt; i++) {
					if (sclient->pfd == spfds[i].fd) {
						clean_pfds(sclients, spfds, i,
						    sdevs);
					}
				}
			}
		}
	}

	if ((tmppfds = realloc(pfds, pfdcnt * sizeof(tmppfds))) == NULL)
		fatal("realloc tmppfds");
	pfds = tmppfds;
}

pid_t
busybee_main(int pipe_prnt[2], int fd_ctl, struct busybeed_conf *xconf,
	     struct s_conf *x_devices, struct sock_conf *x_socks,
	     struct client_conf *x_clients)
{
	pid_t				 pid;

	struct s_device			*ldevs;
	struct s_socket			*lsocks;

	struct busybeed_conf	 	*bconf;
	struct s_conf			*sdevs;
	struct sock_conf		*socks;
	struct client_conf		*sclients;
	struct client_timer_data	*cdata;
	struct devwd_timer_data		*wddata;
	pthread_t			 devwd_thread;

	bconf =				 xconf;
	sdevs =				 x_devices;
	socks =				 x_socks;
	sclients =			 x_clients;

	int				 pi = 0, i, pollsocks;
	int				 rcv, mytype, c_conn = 0;
	int				 n_client = -1, is_client = 0;

	clients_start =			 (sdevs->count + socks->count);
	nfds =				 clients_start;
	pfdcnt =			 (sdevs->count + socks->count +
					      max_clients);

	if ((pfds = calloc(pfdcnt, sizeof(struct pollfd))) == NULL)
		fatal("calloc pfds");
	if ((tmppfds = calloc(pfdcnt, sizeof(struct pollfd))) == NULL)
		fatal("calloc tmppfds");

	/* load up fds */
	TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
		pfds[pi].fd =		 lsocks->listener;
		pfds[pi].events =	 POLLIN;
		pfds[pi++].revents =	 0;
		
	}
	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		pfds[pi].fd =		 ldevs->fd;
		pfds[pi].events =	 POLLIN;
		pfds[pi++].revents =	 0;
	}
	for (i = pi; i < pfdcnt; i++)
		pfds[i].fd = 0;

	memset(sclients, 0, sizeof(sclients));
	if((cdata = (struct client_timer_data *) calloc(1, sizeof(*cdata))) ==
	    NULL)
		fatal("calloc cdata");
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

	close(pipe_prnt[0]);

	// setup device watcher thread
	wddata->seconds = c_retry;
	wddata->quit = &bb_quit;
	wddata->s_devs = sdevs;
	wddata->s_socks = socks;
	wddata->fptr = open_devices;

	if (pledge("stdio tty rpath wpath inet proc", NULL) == -1)
		err(1, "pledge");

	signal(SIGTERM, bb_sighdlr);
	signal(SIGINT, bb_sighdlr);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, bb_sighdlr);
	signal(SIGCHLD, SIG_DFL);

	TAILQ_INIT(&ctl_conns);
	if ((ibuf_main = malloc(sizeof(struct imsgbuf))) == NULL)
		fatal(NULL);
	imsg_init(ibuf_main, pipe_prnt[1]);

	// maybe do a loop read on all pfds to clear them here to begin with

	while (bb_quit == 0) {
		c_nfds = nfds;
		/* start polling */
		pollsocks = poll(pfds, nfds, 1000);
		if (pollsocks == -1)
			fatal("poll() failed");

		for (i = 0; i < c_nfds; i++) {
			if (pfds[i].revents == 0)
				continue;
			/* only check sockets for client connections */
			TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
				if (pfds[i].fd == lsocks->listener) {
					if (pfds[i].revents == POLLIN) {
						is_client = 1;
						break;
					}
				}
			}
			if (is_client == 1) {
				do {
					n_client = accept(lsocks->listener,
								NULL, NULL);
					if (n_client == -1)
						if (errno != EWOULDBLOCK) {
							log_warn("failed");
							bb_quit = 1;
							break;
						}
					if (nfds < pfdcnt) {
						int opts =
						    fcntl(n_client, F_GETFL);
						if (opts < 0)
							log_warn("opts failed");
						opts |= O_NONBLOCK;
						if (fcntl(n_client, F_SETFL,
						    opts) < 0)
							log_warn("F_SETFL");
						pfds[nfds].fd = n_client;
						pfds[nfds].events = POLLIN;
						log_info("client accepted");
						c_client = new_client(n_client);
						c_client->port =
						    lsocks->port;
						TAILQ_INSERT_TAIL(
						    &sclients->clients,
						    c_client, entry);
						cdata->seconds = SUBTIME;
						cdata->fptr = test_client;
						cdata->cconf = sclients;
						cdata->pfd = pfds;
						cdata->c_pfd = n_client;
						nfds++;
						start_client_timer(cdata);
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
					TAILQ_FOREACH(ldevs, &s_devs->s_devices,
					    entry) {
						if (ldevs->fd == pfds[i].fd) {
							mytype = ldevs->type;
							break;
						}
					}
					switch(mytype) {
					case TCP:
						rcv = recv(pfds[i].fd, buff,
						    sizeof(buff), 0);
						break;
					case UDP:
						rcv = recvfrom(pfds[i].fd, buff,
						    sizeof(buff), 0, addr,
						    addrlen);
						/*
						 * nmap to udp triggers rcv to
						 * be 0, so, if local listener,
						 * we don't want it to close.
						 */
						if (rcv == 0 && i <
						    clients_start)
							rcv = 1;
						break;
					case FD:
						rcv = read(pfds[i].fd, buff,
						    sizeof(buff));
						break;
					}
					if (rcv < 0) {
						if (errno != EWOULDBLOCK)
							log_warn("recv() "	
							    "failed");
						if (errno == EBADF)
							log_warn("error");
						break;
					}
					if (rcv == 0) {
						if (i < clients_start)
							wdcheck =
							    pthread_create(
								&devwd_thread,
								NULL, devwd,
								(void *)
								wddata);
						clean_pfds(sclients, pfds, i,
						    sdevs);
						if (wdcheck)
							fatalx("wd thread"
							    "creation failed");
						break;
					}

					ph = packet_handler(sclients, pfds,
					    buff, i, rcv, sdevs);

					if (ph == 0)
						break;

				} while (bb_quit == 0);
			}
		}
		n_client = -1;
	}
	for (i = 0; i < nfds; i++)
	{
		if (pfds[i].fd >= 0)
			close(pfds[i].fd);
	}
	pthread_join(devwd_thread, NULL);
	free(tmppfds);
	free(pfds);
	_exit(0);
}
