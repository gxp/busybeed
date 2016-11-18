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
#include <syslog.h>
#include <unistd.h>



/*#define PATH_CONF	"/etc/busybee.conf"*/
#define PATH_CONF	"busybee.conf"

char		*__progname;	/* from crt0.o */

struct log_file {
	char			log_name[NAME_MAX];
	int			log_fd;
	uint32_t		log_id;
	TAILQ_ENTRY(log_file)	log_entry;
};
TAILQ_HEAD(log_files, log_file) log_files;

/* pseudos */
/* busybee.c */

/* log.c */
void	log_init(int, int);
void	log_procinit(const char *);
void	log_verbose(int);
void	log_warn(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));
void	log_warnx(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));
void	log_info(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));
void	log_debug(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));
void	logit(int, const char *, ...)
__attribute__((__format__ (printf, 2, 3)));
void	vlog(int, const char *, va_list)
__attribute__((__format__ (printf, 2, 0)));
__dead void fatal(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));
__dead void fatalx(const char *, ...)
__attribute__((__format__ (printf, 1, 2)));

/* parse.y */
#define DEFAULT_BAUD		 9600
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

int		parse_config(const char *, struct device *);

/* sockets.c */

/* serial.c */

