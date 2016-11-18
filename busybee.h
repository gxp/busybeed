/*
 * busybee.h
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include <sys/queue.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>



//#define PATH_CONF	"/etc/busybee.conf"
#define PATH_CONF	"busybee.conf"

char		*__progname;	/* from crt0.o */

struct log_file {
	char			log_name[NAME_MAX];
	int			log_fd;
	uint32_t		log_id;
	TAILQ_ENTRY(log_file)	log_entry;
};
TAILQ_HEAD(log_files, log_file) log_files;

//pseudos
// busybee.c

// parse.y

struct device			*new_device(char *);

struct device {
	TAILQ_ENTRY(device)	 entry;
	char			*name;
	int			 port;
	char			*devicelocation;
	int			 baud;
	int			 databits;
	char			*parity;
	int			 stopbits;
	int			 hwctrl;
	char			*password;
};
struct device			*currentdevice;

TAILQ_HEAD(devices, device)	 devices;

int		parse_config(const char *);

// sockets.c

// serial.c
//int open_port();
