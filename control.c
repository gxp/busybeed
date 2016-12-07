/*	$OpenBSD: control.c,v 1.0 2016/12/06 17:12:22 baseprime Exp $ */

/*
 * Copyright (c) 2003, 2004 Henning Brauer <henning@openbsd.org>
 * Copyright (c) 2012 Mike Miller <mmiller@mgm51.com>
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

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
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