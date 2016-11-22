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

#include <sys/queue.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>

#include "busybeed.h"

void		sighdlr(int);
__dead void	usage(void);
int		main(int, char *[]);
int		check_child(pid_t, const char *);

volatile sig_atomic_t	 quit = 0;
volatile sig_atomic_t	 reconfig = 0;
volatile sig_atomic_t	 sigchld = 0;

void
sighdlr(int sig)
{
	switch (sig) {
		case SIGTERM:
		case SIGINT:
			quit = 1;
			break;
		case SIGCHLD:
			sigchld = 1;
			break;
		case SIGHUP:
			reconfig = 1;
			break;
	}
}

__dead void
usage(void)
{
	extern char *__progname;
	
	if (strcmp(__progname, "busybeectl") == 0)
		fprintf(stderr,
			"usage: busybeectl -n name -d /dev/device etc\n");
		else
			fprintf(stderr, "usage: %s [-dnSsv] [-f file]\n",
				__progname);
			exit(1);
}

int
main(int argc, char *argv[])
{

	struct s_conf			 sdevs;
	struct busybeed_conf	 	 lconf;
	memset(&sdevs, 0, sizeof(sdevs));
	memset(&lconf, 0, sizeof(lconf));

	/* log to stderr until daemonized */
	log_init(lconf.debug ? lconf.debug : 1, LOG_DAEMON);
	
	if (parse_config(PATH_CONF, &lconf))
		exit(1);

	if (open_devices(&sdevs))
		exit(1);
	
	return 0;
}
