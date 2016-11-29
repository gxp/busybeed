/* $OpenBSD: busybeed.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
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

/* I've taken and learned a lot from ntpd. Thank you! */

#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "busybeed.h"

void			 sighdlr(int);
__dead void		 usage(void);
int			 main(int, char *[]);
void			 ctl_main(int, char*[]);

int			 verbose = 1;
volatile sig_atomic_t	 quit = 0;

void
sighdlr(int sig)
{
	switch (sig) {
		case SIGQUIT:
		case SIGTERM:
		case SIGINT:
		case SIGHUP:
			quit = 1;
			break;
	}
}

__dead void
usage(void)
{
	extern char *__progname;
	
	if (strcmp(__progname, "busybctl") == 0)
		fprintf(stderr,
			"usage: busybctl -n name -d /dev/device etc\n");
		else
			fprintf(stderr, "usage: %s [-d]\n",
				__progname);
			exit(1);
}

int
main(int argc, char *argv[])
{

	struct busybeed_conf	 	 lconf;
	struct s_conf			 sdevs;
	struct s_device			*ldevs;
	struct sock_conf		 socks;
	struct s_socket			*lsocks;

	int				 pipe_chld[2];
	pid_t				 chld_pid = 0;
	int				 fd_ctl, ch, bbdm = 0;

	if (strcmp(__progname, "busybctl") == 0) {
		ctl_main(argc, argv);
		/* NOTREACHED */
	}

	memset(&lconf, 0, sizeof(lconf));
	while ((ch = getopt(argc, argv, "dv")) != -1) {
		switch (ch) {
			case 'd':
				lconf.debug = 1;
				break;
			case 'v':
				lconf.verbose++;
				break;
			default:
				usage();
				/* NOTREACHED */
		}
	}

	memset(&sdevs, 0, sizeof(sdevs));
	memset(&socks, 0, sizeof(socks));
	/* log to stderr until daemonized */
	log_init(lconf.debug ? lconf.debug : 1, LOG_DAEMON);

	argc -= optind;
	argv += optind;
	
	if (geteuid()) {
		errx(1, "need root privileges");
		exit(1);
	}

	if (parse_config(PATH_CONF, &lconf))
		exit(1);

	if (open_devices(&sdevs))
		exit(1);

	if (create_sockets(&socks, &sdevs))
		exit(1);

	if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, pipe_chld) == -1)
		fatal("socketpair");

	if ((fd_ctl = control_init(CTLSOCKET)) == -1)
		fatalx("control socket init failed");

	if (control_listen(fd_ctl) == -1)
		fatalx("control socket listen failed");

	/* fork child process */
	chld_pid = busybee_main(pipe_chld, fd_ctl, &lconf, &sdevs, &socks);
	
	log_procinit("[priv]");
	
	/*exit(EXIT_SUCCESS);	*/
	signal(SIGQUIT, sighdlr);
	signal(SIGTERM, sighdlr);
	signal(SIGINT, sighdlr);
	signal(SIGHUP, sighdlr);

	close(pipe_chld[1]);

	if (pledge("stdio tty rpath wpath inet proc",
		NULL) == -1)
		err(1, "pledge");

	while (quit == 0) {
		if (bbdm == 0) {
			bbdm = 1;
			log_init(lconf.debug, LOG_DAEMON);
			log_verbose(lconf.verbose);
			log_info("busybeed started");
			if (!lconf.debug)
				if (daemon(1, 0))
					fatal("daemon");
		}
		
		/* busybctl crap in here eventually */
		/* write and recv messages with child */
		sleep(20000);
	}


	if (chld_pid)
		kill(chld_pid, SIGTERM);
	
	/* cleanup sockets */
	TAILQ_FOREACH(lsocks, &s_socks->s_sockets, entry) {
		shutdown(lsocks->listener, 2);
		/*close(lsocks->listener);*/
	}
	
	/* cleanup devices */
	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		close(ldevs->fd);
	}

	log_info("busybeed terminating");
	return 0;
}


void
ctl_main(int argc, char *argv[])
{
	char			*sockname;
	
	sockname = CTLSOCKET;
	
/* busybctl crap
 * add devices
 * remove devices
 * look at devices
 * look at clients
 * look at client subscriptions
 * kick clients
 */

	printf("Reached control!");
	exit(0);



}