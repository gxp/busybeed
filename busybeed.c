/*
 * busybeed.c
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include "busybeed.h"

extern char		*__progname;	/* from crt0.o */

int
main(int argc, char *argv[])
{
	struct device		*devs;
	struct busybee_conf	 lconf;
	memset(&lconf, 0, sizeof(lconf));

	if (parse_config(PATH_CONF, &lconf))
		exit(1);

	TAILQ_FOREACH(devs, &conf->devices, entry) {
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
	return 0;
}
