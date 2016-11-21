/* $OpenBSD: busybeed.h v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
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

#include <sys/types.h>
#include <sys/queue.h>
#include <stdarg.h>


/*#define PATH_CONF	"/etc/busybee.conf"*/
#define PATH_CONF	"busybee.conf"



/* prototypes */

/* busybeed.c */
extern struct busybeed_conf 	*conf;
extern char			*__progname;	/* from crt0.o */
extern int			 max_clients;

/* client.c */

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

struct busybeed_conf {
	TAILQ_HEAD(devices, device)	 devices;
	int		        	 debug;
};

int				parse_config(const char *,
					struct busybeed_conf *);

/* sockets.c */

/* serial.c */

extern struct s_conf		*s_devs;
extern int			 open_devices(struct s_conf *);

struct s_device {
	TAILQ_ENTRY(s_device)	 entry;
	int				 fd;
	char				*name;
};

struct s_conf {
	TAILQ_HEAD(s_devices, s_device)		s_devices;
	int					s_count;
	int					s_front;
};