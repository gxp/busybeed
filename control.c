/*	$OpenBSD: control.c,v 1.0 2016/12/06 17:12:22 baseprime Exp $ */

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2012 Mike Miller <mmiller@mgm51.com>
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

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "busybeed.h"

#define	CONTROL_BACKLOG	5

int
control_init(char *path)
{
	struct sockaddr_un	 sa;
	int			 fd;
	mode_t			 old_umask;
	
	if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		log_warn("control_init: socket");
		return (-1);
	}
	
	memset(&sa, 0, sizeof(sa));
	sa.sun_family = AF_UNIX;
	if (strlcpy(sa.sun_path, path, sizeof(sa.sun_path)) >=
		sizeof(sa.sun_path))
		errx(1, "ctl socket name too long");
	
	if (unlink(path) == -1)
		if (errno != ENOENT) {
			log_warn("control_init: unlink %s", path);
			close(fd);
			return (-1);
		}
		
	old_umask = umask(S_IXUSR|S_IXGRP|S_IWOTH|S_IROTH|S_IXOTH);
	if (bind(fd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
		log_warn("control_init: bind: %s", path);
		close(fd);
		umask(old_umask);
		return (-1);
	}
	umask(old_umask);
	
	if (chmod(path, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP) == -1) {
		log_warn("control_init: chmod");
		close(fd);
		(void)unlink(path);
		return (-1);
	}

	session_socket_blockmode(fd, BM_NONBLOCK);
	
	return (fd);
}

int
control_listen(int fd)
{
	if (fd != -1 && listen(fd, CONTROL_BACKLOG) == -1) {
		log_warn("control_listen: listen");
		return (-1);
	}
	
	return (0);
}

void
control_shutdown(int fd)
{
	close(fd);
}

void
control_cleanup(const char *path)
{
	if (path)
		unlink(path);
}

int
control_accept(int listenfd)
{
	int			 connfd;
	socklen_t		 len;
	struct sockaddr_un	 sa;
	struct ctl_conn		*ctl_conn;

	len = sizeof(sa);
	if ((connfd = accept(listenfd, (struct sockaddr *)&sa, &len)) == -1) {
		if (errno != EWOULDBLOCK && errno != EINTR)
			log_warn("control_accept: accept");
		return (0);
	}
	session_socket_blockmode(connfd, BM_NONBLOCK);

	if ((ctl_conn = calloc(1, sizeof(struct ctl_conn))) == NULL) {
		log_warn("control_accept");
		close(connfd);
		return (0);
	}

	imsg_init(&ctl_conn->ibuf, connfd);
	
	TAILQ_INSERT_TAIL(&ctl_conns, ctl_conn, entry);

	return (1);
}

struct ctl_conn *
control_connbyfd(int fd)
{
	struct ctl_conn	*c;
	
	for (c = TAILQ_FIRST(&ctl_conns); c != NULL && c->ibuf.fd != fd;
	     c = TAILQ_NEXT(c, entry))
	     ;	/* nothing */
	     
	     return (c);
}

int
control_close(int fd)
{
	struct ctl_conn	*c;
	
	if ((c = control_connbyfd(fd)) == NULL) {
		log_warn("control_close: fd %d: not found", fd);
		return (0);
	}
	
	msgbuf_clear(&c->ibuf.w);
	TAILQ_REMOVE(&ctl_conns, c, entry);
	
	close(c->ibuf.fd);
	free(c);
	
	return (1);
}

void
session_socket_blockmode(int fd, enum blockmodes bm)
{
	int	flags;
	
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		fatal("fcntl F_GETFL");
	
	if (bm == BM_NONBLOCK)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;
	
	if ((flags = fcntl(fd, F_SETFL, flags)) == -1)
		fatal("fcntl F_SETFL");
}

int
control_dispatch_msg(struct pollfd *pfd, u_int *ctl_cnt)
{
// 	struct imsg		 imsg;
	struct ctl_conn		*c;
// 	int			 cnt;
	ssize_t			 n;
	
	if ((c = control_connbyfd(pfd->fd)) == NULL) {
		log_warn("control_dispatch_msg: fd %d: not found", pfd->fd);
		return (0);
	}
	
	if (pfd->revents & POLLOUT)
		if (msgbuf_write(&c->ibuf.w) <= 0 && errno != EAGAIN) {
			*ctl_cnt -= control_close(pfd->fd);
			return (1);
		}
		
	if (!(pfd->revents & POLLIN))
		return (0);
	
	if (((n = imsg_read(&c->ibuf)) == -1 && errno != EAGAIN) || n == 0) {
		*ctl_cnt -= control_close(pfd->fd);
		return (1);
	}
	/*
	for (;;) {
		if ((n = imsg_get(&c->ibuf, &imsg)) == -1) {
			*ctl_cnt -= control_close(pfd->fd);
			return (1);
		}
		if (n == 0)
			break;
		
		switch (imsg.hdr.type) {
		case IMSG_CTL_SHOW_STATUS:
			build_show_status(&c_status);
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_STATUS, 0, 0, -1,
					&c_status, sizeof (c_status));
			break;
		case IMSG_CTL_SHOW_PEERS:
			cnt = 0;
			TAILQ_FOREACH(p, &conf->ntp_peers, entry) {
				build_show_peer(&c_peer, p);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS,
				    0, 0, -1, &c_peer, sizeof(c_peer));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS_END,
					0, 0, -1, &cnt, sizeof(cnt));
			break;
		case IMSG_CTL_SHOW_SENSORS:
			cnt = 0;
			TAILQ_FOREACH(s, &conf->ntp_sensors, entry) {
				build_show_sensor(&c_sensor, s);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS,
				    0, 0, -1, &c_sensor, sizeof(c_sensor));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS_END,
			    0, 0, -1, &cnt, sizeof(cnt));
			break;
		case IMSG_CTL_SHOW_ALL:
			build_show_status(&c_status);
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_STATUS, 0, 0, -1,
			    &c_status, sizeof (c_status));
			
			cnt = 0;
			TAILQ_FOREACH(p, &conf->ntp_peers, entry) {
				build_show_peer(&c_peer, p);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS,
				    0, 0, -1, &c_peer, sizeof(c_peer));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_PEERS_END,
					0, 0, -1, &cnt, sizeof(cnt));
			
			cnt = 0;
			TAILQ_FOREACH(s, &conf->ntp_sensors, entry) {
				build_show_sensor(&c_sensor, s);
				imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS,
				    0, 0, -1, &c_sensor, sizeof(c_sensor));
				cnt++;
			}
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_SENSORS_END,
			    0, 0, -1, &cnt, sizeof(cnt));
			
			imsg_compose(&c->ibuf, IMSG_CTL_SHOW_ALL_END,
			    0, 0, -1, NULL, 0);
			break;
		default:
			break;
		}
		imsg_free(&imsg);
	}*/
	return (0);
}