/* $OpenBSD: busybeed.h v.1.03 2017/11/11 08:57:00 baseprime Exp $ */ 
/*
 * Copyright (c) 2016 Tracey Emery <tracey@traceyemery.net>
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
#include <sys/uio.h>

#include <imsg.h>
#include <poll.h>
#include <pthread.h>
#include <stdarg.h>

#define PATH_CONF	"/etc/busybeed.conf"
#define CTLSOCKET	"/var/run/busybeed.sock"
#define BUFFRSIZE	 1024
#define DEFAULTRETRY	 30
enum blockmodes {
	BM_NORMAL,
	BM_NONBLOCK
};
extern int			 max_clients, max_subscriptions;

/* prototypes */
/* busybeed.c */
extern struct busybeed_conf 	*conf;

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
extern char			*bind_interface;
struct device			*new_device(char *);
struct device {
	TAILQ_ENTRY(device)	 entry;
	char			*name;
	char			 port[6];
	int			 max_clients;
	int			 cport;
	char			*devicelocation;
	char			*ipaddr;
	char			*udp;
	int			 baud;
	int			 databits;
	char			*parity;
	int			 stopbits;
	int			 hwctrl;
	int			 swctrl;
	char			*password;
	char			*bind_interface;
	int			 listener;
	int			 persistent;
};
struct device			*currentdevice;
struct busybeed_conf {
	TAILQ_HEAD(devices, device)	 devices;
	int		        	 debug;
	int		        	 verbose;
};
int				 parse_config(const char *,
				     struct busybeed_conf *);

/* serial.c */
enum sock {
	TCP,
	UDP,
	FD
} stype;
extern struct s_conf		*s_devs;
struct s_device			*new_s_device(char *);
struct s_device {
	TAILQ_ENTRY(s_device)	 entry;
	int			 fd;
	char			 port[6];
	int			 cport;
	int			 max_clients;
	int			 subscribers;
	int			 listener;
	int			 persistent;
	char			*bind_interface;
	char			*ipaddr;
	char			*udp;
	char			*location;
	char			*password;
	char			*name;
	int			 connected;
	int			 type;
};
struct s_device			*cs_device;
struct s_conf {
	TAILQ_HEAD(s_devices, s_device)		 s_devices;
	int					 count;
};

/* sockets.c */
extern struct sock_conf		*s_socks;
extern int			 create_sockets(struct sock_conf *,
				     struct s_conf *, char *);
extern char			*get_ifaddrs(char *);
struct s_socket			*new_socket(char *);
struct s_socket {
	TAILQ_ENTRY(s_socket)	 entry;
	char			 port[6];
	int			 listener;
};
struct s_socket			*c_socket;
struct sock_conf {
	TAILQ_HEAD(s_sockets, s_socket)		 s_sockets;
	int					 count;
};
extern int			 open_devices(struct s_conf *,
				     struct s_device *, struct sock_conf *);
int				 create_socket(char *, char *, int);
int				 open_client_socket(char *, int);

/* client.c */
struct client {
	TAILQ_ENTRY(client)	 entry;
	char			*name;
	int			 pfd;
	int			 subscribed;
	pthread_t		 me_thread;
	int			 lastelement;
	char			*port;
	char			**subscriptions_name;
	int			 subscriptions[];
};
struct client			*c_client;
struct client_conf {
	TAILQ_HEAD(clients, client)	 	clients;
};
int				 client_subscribe(struct client_conf *, int,
				     u_char *);
void				 test_client(struct pollfd *,
				     struct client_conf *);
struct client_timer_data {
	int			 seconds;
	void			 (*fptr)(struct pollfd *pfd,
				     struct client_conf *cconf);
	struct client_conf	*cconf;
	struct pollfd		*pfd;
	int			 c_pfd;
};
void				 start_client_timer(struct client_timer_data *);
void				*run_client_timer(void *data);
struct client			*new_client(int);
void				 do_subscribe(int, char *, int,
				     struct client_conf *);
/* busybee.c */
#define SUBTIME			 5
void				 clean_devs(int[], struct s_conf *);
void				 clean_pfds(struct client_conf *,
				     struct pollfd *, int, struct s_conf *);

pid_t				 busybee_main(int[2], int,
				     struct busybeed_conf *,struct s_conf *,
				     struct sock_conf *, struct client_conf *);
int				 packet_handler(struct client_conf *,
				    struct pollfd *, u_char *, int, int,
				    struct s_conf *);
int				 parse_buffer(struct client_conf *, u_char *,
				     int); /* parse.y member */
void				 write_packet(int, int, char *, u_char *,
				     struct s_conf *);
extern struct ctl_conns 	 ctl_conns;

/* control.c */
struct ctl_conn {
	TAILQ_ENTRY(ctl_conn)	entry;
	struct imsgbuf		ibuf;
};
TAILQ_HEAD(ctl_conns, ctl_conn)	;
int			 	 control_init(char *);
int			 	 control_listen(int);
void			 	 control_shutdown(int);
void			 	 control_cleanup(const char *);
int			 	 control_accept(int);
void				 session_socket_blockmode(int, enum blockmodes);
int				 control_close(int);
struct ctl_conn			*control_connbyfd(int);
int				 control_dispatch_msg(struct pollfd *, u_int *);

/* devwd.c */
pthread_mutex_t			 wdlock;
void				*devwd(void *data);
struct devwd_timer_data {
	int			 seconds;
	volatile sig_atomic_t	*quit;
	struct s_conf		*s_devs;
	struct sock_conf	*s_socks;
	struct s_device		*ldevs;
	struct s_socket		*lsocks;
	struct pollfd		*pfds;
	int			 (*fptr)(struct s_conf *, struct s_device *,
				     struct sock_conf *);
};
