/*
 * busybee.c
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include "busybee.h"

int
main(int argc, char *argv[])
{
	int debug = 0;
	int c;

	c = getopt (argc, argv, "h:");
		
	struct device		*devs;
	TAILQ_INIT(&devices);
	memset(&devs, 0, sizeof(devs));
	if (parse_config(PATH_CONF, &devs) == -1)
		exit(1);
		


	log_init(debug ? debug : 1, LOG_DAEMON);
	
	TAILQ_FOREACH(devs, &devices, entry) {
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

	/* Start daemon fork here */



	/* Serial */

	
	/* fine */
	return 0;
}
