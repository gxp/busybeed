/*
 * busybee.c
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include "busybee.h"


extern int		*parse_config(const char *);

//char *devices, *device[6];

int main(int argc, char *argv[]) {
printf("Hello");
	// Load config
//	char *config = get_conf();

//strsep for split of config

//	puts(config);

	// Start daemon fork here



	// Serial

}


/*
char *get_conf() {
	long fsize;
	const char *c_file = "/etc/busybee.conf";
	char *c_read = "0";
	if (access(c_file, F_OK) != -1) {
		FILE *c = fopen(c_file, "r");
		fseek(c, 0, SEEK_END);
		fsize = ftell(c);
		fseek(c, 0, SEEK_SET);
		c_read = malloc(fsize+1);
		fread(c_read, fsize, 1, c);
		fclose(c);
		return c_read;
	} else {
		exit(EXIT_FAILURE);
	}
}
*/