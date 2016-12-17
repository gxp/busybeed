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
	 * this isn't quite right yet -- {"subscribe":
	 * 	[{"name":"ClientName"},
	 * 	{"devices":
	 * 		[{"device":"dev1","password":"12345678"},
	 * 		{"device":"dev2","password":"12345678"},
	 * 		{"device":"dev3","password":"12345678"}
	 * 	]}
	 * ]}
	 * 
	 * ~~~{"subscribe":[{"name":"ClientName"},{"devices":[{"device":"dev1","password":"12345678"}]}]}
	 * Tabs and returns entered for readability. JSON request should be on
	 * one line.
	 * 
	 * ~~~{"subscribe":{"name":"ClientName"},"devices":[{"device":"dev1","password":"12345678"},{"device":"dev2","password":"12345678"}]}
	 */

	struct json_object	*json_obj, *sub_obj, *name_obj, *dev_obj;
	//enum json_type		 type;
	const char			*my_name;
	unsigned char		*s_buff;
	int			 j_exists;

	s_buff =		 x_buff;
	memmove(s_buff, s_buff+3, strlen(s_buff+3)+1);

	/* Test curly brace on first character. After testing json-c, it seems
	 * null is not returned on the tokener if numbers are passed.
	 * ~~~sadface fails
	 * ~~~1234567 passes crashing the child with no error
	 */
	if (s_buff[0] != '{')
		return -1;

	json_obj =		 json_tokener_parse(s_buff);

	if (is_error(json_obj)) {
		return -1;
	}

	/* validate subscribe packet */
	j_exists = json_object_object_get_ex(json_obj, "subscribe", &sub_obj);
	if (j_exists == 0) {
		log_warnx("not a subscribe packet");
		return -1;
	}
	/* get client name */
	j_exists = json_object_object_get_ex(sub_obj, "name", &name_obj);
	if (j_exists == 0) {
		log_warnx("no client name");
		return -1;
	} else {
		my_name = json_object_get_string(name_obj);
	}
	/* grab devices */
	j_exists = json_object_object_get_ex(json_obj, "devices", &dev_obj);
	if (j_exists == 0) {
		log_warnx("no devices");
		return -1;
	} else {
		printf("Devices: %s\n", json_object_get_string(dev_obj));
		return 0;
	}
	/*json_object_object_foreach(json_obj, key, val) {
		if (strcmp(key, "subscribe") == 0) {
			json_obj =	 json_object_get(val);
			json_object_object_foreach(json_obj, keya, vala) {
				printf("Ka: %s\n", keya);
				if (strcmp(keya, "name") == 0) {
					
					printf("Name: %s\n", my_name);
					printf("N2: %s\n",json_object_get_string(vala));
					
				
				}
				return 0;
			}
			return -1;
		}
	}*/
	return -1;
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

