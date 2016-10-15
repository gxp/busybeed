/*
 * serial.c
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include "busybee.h"

int get_conf() {

	char conf_r[255];
	long fsize;
	FILE *conf;

	*conf= fopen("/etc/busybee.conf", "r");
	fseek(conf, 0, SEEK_END);
	fsize; = ftell(conf);
	fseek(conf, 0, SEEK_SET);

	fclose(conf);
}
int open_port() {



	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return 0;
}
