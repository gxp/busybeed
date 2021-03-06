/* $OpenBSD: devwd.c v.1.01 2017/11/11 08:57:00 baseprime Exp $ */ 
/*
 * Copyright (c) 2017 Tracey Emery <tracey@traceyemery.net>
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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "busybeed.h"

extern pthread_mutex_t wdlock;

void
*devwd(void *data)
{
	pthread_mutex_lock(&wdlock);
	int (*t_fptr)(struct s_conf *, struct s_device *, struct sock_conf *);
	struct devwd_timer_data		*wddata;
	struct s_device			*ldevs;
	int				 od, connd;
	connd = 0;
	wddata = data;
	t_fptr = wddata->fptr;
	while(*wddata->quit == 0) {
		sleep(wddata->seconds);
		TAILQ_FOREACH(ldevs, &wddata->s_devs->s_devices, entry) {
			if (ldevs->connected == 0) {
				od = (int) (*t_fptr)(wddata->s_devs, ldevs,
				    wddata->s_socks);
				connd = 1;
				continue;
			}
		}
		if (connd)
			break;
	}
	pthread_mutex_unlock(&wdlock);
	return NULL;
}

