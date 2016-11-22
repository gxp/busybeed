/* $OpenBSD: serial.c v.1.00 2016/11/20 14:59:17 baseprime Exp $ */
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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "busybeed.h"

struct s_conf			*s_devs;

int
open_devices(struct s_conf *x_devs)
{
	int				 fd;
	int				 baudrate = 0;
	int				 stop = 0;

	struct		termios s_opts;
	
	s_devs = x_devs;
	TAILQ_INIT(&s_devs->s_devices);
	
	struct device		*devs;
	TAILQ_FOREACH(devs, &conf->devices, entry) {
		cs_device = new_s_device(devs->name);
		
		fd = open(devs->devicelocation, O_RDWR | O_NOCTTY | O_NDELAY);
		if (fd == -1) {
			log_warnx("failed to open %s", devs->devicelocation);
			return(-1);
		} else {
			/* normal blocking */
			fcntl(fd, F_SETFL, 0);
			/* load current s_opts */
			tcgetattr(fd, &s_opts);
			/* set baud */
			switch(devs->baud) {
				case 50:
					baudrate = B50;
					break;
				case 75:
					baudrate = B75;
					break;
				case 110:
					baudrate = B110;
					break;
				case 134:
					baudrate = B134;
					break;
				case 150:
					baudrate = B150;
					break;
				case 200:
					baudrate = B200;
					break;
				case 300:
					baudrate = B300;
					break;
				case 600:
					baudrate = B600;
					break;
				case 1200:
					baudrate = B1200;
					break;
				case 1800:
					baudrate = B1800;
					break;
				case 2400:
					baudrate = B2400;
					break;
				case 4800:
					baudrate = B4800;
					break;
				case 9600:
					baudrate = B9600;
					break;
				case 19200:
					baudrate = B19200;
					break;
				case 38400:
					baudrate = B38400;
					break;
				case 57600:
					baudrate = B57600;
					break;
				case 76800:
					baudrate = B76800;
					break;
				case 115200:
					baudrate = B115200;
					break;
			}
			cfsetispeed(&s_opts, baudrate);
			cfsetospeed(&s_opts, baudrate);
			/* enable and set local */
			s_opts.c_cflag |= (CLOCAL | CREAD);
			/* set data bits */
			if (devs->databits != -1) {
				s_opts.c_cflag &= ~CSIZE;
				switch(devs->databits) {
					case 5:
						stop = CS5;
						break;
					case 6:
						stop = CS6;
						break;
					case 7:
						stop = CS7;
						break;
					case 8:
						stop = CS8;
						break;
				}
				s_opts.c_cflag |= stop; 
			}
			/* set parity */
			if (devs->parity != '\0') {
				s_opts.c_cflag &= ~PARENB;
				/* enable parity checking */
				if (strcmp(devs->parity, "odd") == 0) {
					s_opts.c_cflag |= PARENB;
					s_opts.c_cflag |= PARODD;
					s_opts.c_iflag |= (INPCK | ISTRIP);
				} else if (strcmp(devs->parity, "even") == 0) {
					s_opts.c_cflag |= PARENB;
					s_opts.c_cflag &= ~PARODD;
					s_opts.c_iflag |= (INPCK | ISTRIP);
				}
			}
			/* set stop bits */
			if (devs->stopbits != -1) {
				if (devs->stopbits == 2) {
					s_opts.c_cflag |= CSTOPB;
				} else {
					s_opts.c_cflag &= ~CSTOPB;
				}
			}
			/* set hardware control */
			/* doesn't seem to be enabled in OpenBSD */
			if (devs->hwctrl != -1) {
				/* s_opts.c_cflag &= ~CNEW_RTSCTS; */
				if (devs->hwctrl == 1) {
					/* s_opts.c_cflag |= CNEW_RTSCTS; */
				}
			}
			/* set software control */
			if (devs->swctrl != -1) {
				s_opts.c_iflag &= ~(IXON | IXOFF | IXANY);
				if (devs->swctrl == 1) {
					s_opts.c_iflag |=
						(IXON | IXOFF | IXANY);
				}	
			}
			/* set input/output as raw */
			s_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
			s_opts.c_oflag &= ~OPOST;
			/* Set the new options for the port */
			tcsetattr(fd, TCSANOW, &s_opts);
			cs_device->password =		 devs->password;
			cs_device->location =		 devs->devicelocation;
			cs_device->port =		 devs->port;
			cs_device->fd =			 fd;
			TAILQ_INSERT_TAIL(&s_devs->s_devices, cs_device, entry);
		}
	}
	return 0;
}

struct s_device *
new_s_device(char *name)
{
	struct s_device	*dev;
	
	if ((dev = calloc(1, sizeof(*dev))) == NULL)
		fatalx("no s_dev calloc");
	
	if ((dev->name = strdup(name)) == NULL)
		fatalx("no s_dev name");
	
	return (dev);
}

