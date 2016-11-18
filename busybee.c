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


	struct device		*devs;
	TAILQ_INIT(&devices);
	
	if (parse_config(PATH_CONF) == -1)
		//exit(1);
		



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

	// Start daemon fork here



	// Serial

}
