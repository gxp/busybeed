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

#include "busybeed.h"

/* tail q of clients ... what shall we store? */
/* connected socket fd, subscribed socket fd */
/* password will already have been used, don't need that */
/* client addr_info! */

int			 max_clients = 1, max_subscriptions = 1;

/*
 * client subscriber, something like subscribe_client(char *clientname,
 * 					char *devname, char *password);
 */

/*
 * clients can connect to a socket and THEN subscribe to a stream via name and password
 * clients should only be allowed to test a password 3 times before being kicked out
 * we could even add "bad" clients to pf rules
 *
 */
/*
 * 	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		
		client will need this info later. keep here for now
		printf("Name: %s\n", ldevs->name);
		printf("Port: %i\n", ldevs->port);
		printf("Location: %s\n", ldevs->location);
		printf("Password: %s\n", ldevs->password); 
		

	}
*/
