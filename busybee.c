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

#include <err.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "busybeed.h"

volatile sig_atomic_t		 bb_quit = 0;
void				 bb_sighdlr(int);

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
busybee_main(int pipe_prnt[2], int fd_ctl, struct busybeed_conf *bconf,
	     struct s_conf * x_devices, struct sock_conf *x_socks)
{
	pid_t			 pid;

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
		/* poll for connections and setup clients */
		sleep(10000);
	}
	_exit(0);
}