/* $OpenBSD: serial.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>

#include "busybeed.h"

struct s_conf			*s_devs;

int
open_devices(struct s_conf *x_devs)
{

	s_devs = x_devs;
	TAILQ_INIT(&s_devs->s_devices);
	
	struct device		*devs;
	TAILQ_FOREACH(devs, &conf->devices, entry) {
		/* iterate, setup with options, add to queue  */
		
		printf("Name: %s\n", devs->name);
		printf("Port: %i\n", devs->port);
		printf("Dev: %s\n", devs->devicelocation);
		printf("Baud: %i\n", devs->baud);
		printf("Data: %i\n", devs->databits);
		printf("Parity: %s\n", devs->parity);
		printf("Stop: %i\n", devs->stopbits);
		printf("Hardware: %i\n", devs->hwctrl);
		printf("Password: %s\n\n", devs->password);
	}

	return 1;
}

