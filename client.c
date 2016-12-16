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

#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "json.h"
#include "busybeed.h"

int
client_subscribe(int pfd, unsigned char *x_buff)
{
	/*
	 * All clients within the client socket portion of pollfd is required
	 * to send a subscribe packet. The packet format is as follows:
	 * 
	 * Three bytes of 0x7E followed by the JSON subscription request.
	 * 
	 * JSON subscription request format:
	 * 
	 * {"subscribe":[
	 * 	{"name":"ClientName"},
	 * 	{"device":"dev1","password":"12345678"},
	 * 	{"device":"dev2","password":"12345678"},
	 * 	{"device":"dev3","password":"12345678"}
	 * ]}
	 * 
	 * ~~~{"subscribe":[{"name":"xbee_listener"},{"device":"data_xbee","password":"password"}]}
	 * Tabs and returns entered for readability. JSON request should be on
	 * one line.
	 */

	json_object			*json_obj;
	unsigned char			*s_buff;

	s_buff =			 x_buff;
	memmove(s_buff, s_buff+3, strlen(s_buff+3)+1);

	json_obj =			 json_tokener_parse(s_buff);
	printf("Subscribe here\n");
	printf("Packet:\n\n%s\n\n", json_object_to_json_string(json_obj));
	return 0;
}

struct client *
new_client(int pfd)
{
	struct client	*client;
	
	if ((client = calloc(1, sizeof(*client))) == NULL)
		fatalx("no client calloc");
	
	if ((client->pfd = pfd) < 1)
		fatalx("no client pfd");
	
	return (client);
};



/*
 * 	TAILQ_FOREACH(ldevs, &s_devs->s_devices, entry) {
		
		client will need this info later. keep here for now
		printf("Name: %s\n", ldevs->name);
		printf("Port: %i\n", ldevs->port);
		printf("Location: %s\n", ldevs->location);
		printf("Password: %s\n", ldevs->password); 
		

	}
*/

