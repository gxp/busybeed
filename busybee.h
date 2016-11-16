/*
 * busybee.h
 *
 *  Created on: Oct 14, 2016
 *      Author: baseprime
 */

#include <stdio.h> 		// everything
#include <stdlib.h>		// everything
#include <string.h>		// everything
#include <stdarg.h>		// everything
#include <unistd.h>		// UNIX standard function definitions

#include <fcntl.h>		// File control definitions - serial.c
#include <errno.h>		// Error number definitions - serial.c
#include <termios.h>		// POSIX terminal control definitions - serial.c

#define PATH_CONF	"/etc/busybee.conf"

//pseudos
// busybee.c

// sockets.c

// serial.c
int open_port();
