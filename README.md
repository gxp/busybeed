# busybeed
The busybeed OpenBSD daemon provides a mechanism for clients and client
processes to communicate with an array of serial and IoT devices. At
its core, busybeed is primarily a packet repeater in that it polls for
packets to swap between subscriber clients and devices. However,
busybeed also provides password control over those connections,
including client limits.

### Installing

busybeed installation

```
cd /usr/src/usr.sbin
git clone https://github.com/spoollord/busybeed.git

- development version
git -b develop clone https://github.com/spoollord/busybeed.git

cd busybeed
make
make -n install
make install

cp examples/etc/rc.d/busybeed /etc/rc.d/
cp examples/etc/examples/busybeed.conf /etc/

man busybeed.conf
nano /etc/busybeed.conf

rcctl enable busybeed
rcctl start busybeed
```

---------------------------------------------------
Man Pages
---------------------------------------------------

NAME

	busybeed -- busybee daemon

SYNOPSIS

	busybeed [-d]

DESCRIPTION

	The busybeed OpenBSD daemon provides a mechanism for clients and client
	processes to communicate with an array of serial and IoT devices. At
	its core, busybeed is primarily a packet repeater in that it polls for
	packets to swap between subscriber clients and devices. However,
	busybeed also provides password control over those connections,
	including client limits.

	busybeed can connect directly to serial devices, IoT IP devices, and
	single socket connections.
	
	The options are as follows:

	-d	Do not daemonize. If this option is specified, busybeed will run
		in the foreground and log to stderr.

	-v	This option allows busybeed to send DEBUG priority messages to
		syslog.

	Example devices tested: XBee, NF24, ESP8266.

	Subscribers to a device pass a simple, human-readable packet to
	busybeed. The format is as follows, without the line breaks and tabs.
	This example is only for initial readability and help in understanding
	the format!

	Subscription packets must begin with 3 * 0x7E

	~~~subscribe{
		{name,"CLIENTNAME"},
		{devices{
			device{"dev1","password1"},
			device{"dev2","password2"}
			}
		}
	}

	An accurate example packet:

	~~~subscribe{{name,"CLIENTNAME"},{devices{device{"dev1","password1"}}}}

	busybeed is usually started at boot time, and can be enabled by setting
	adding busybeed to the pkg_scripts line in /etc/rc.conf.local. See rc(8)
	and rc.conf(8) for more information on the boot process and enabling
	daemons.

	When busybeed starts up, it reads settings from its configuration file,
	typically busybeed.conf(5), and creates sockets and file descriptor
	connections.

FILES

	/etc/busybeed.conf	Default configuration file.
	/var/run/busybeed.sock	Socket for communication with busybctl(8).

SEE ALSO

	busybeed.conf(5), busybctl(8), rc(8), rc.conf(8)

STANDARDS

HISTORY

	The busybeed program first began development on OpenBSD 6.0.

---------------------------------------------------

NAME

	busybeed.conf - busybeed daemon configuration file

DESCRIPTION

	busybeed.conf is the configuration file for the busybee daemon,
	busybeed(8).

SECTIONS

	busybeed.conf is divided into two main sections:

Global Configuration

	Global settings for busybeed(8).  Do note that global settings
	should never be placed after the devices section has been defined.

Devices

	Devices are the configuration areas for each device a client can
	communicate with.

	The current line can be extended over multiple lines using a backslash
	(`\').  Comments can be put anywhere in the file using a hash mark (`#'),
	and extend to the end of the current line.  Care should be taken when
	commenting out multi-line text: the comment is effective until the end of
	the entire block.

	Argument names not beginning with a letter, digit, or underscore must be
	quoted.

GLOBAL CONFIGURATION

	Here are the settings that can be set globally:

default port number

	Set the default port for devices to communicate on. This is a
	required setting.

bind interface string

	Specify a single interface to create sockets on. If this is not
	et, busybeed(8) will attempt to open the default port on all
	network interfaces.

max clients number

	Set the maximum number of clients allowed to connect to all
	busybeed(8) socket.

max subscriptions number

	Set the maximum number of devices a client can subscribe to at a
	time.

connection retry number
	Set the lost connection device watchdog interval. Default is 30
	seconds. Range is 1-600 seconds.

log verbose number

	Turn on verbose logging. (This option is currently disabled.)

DEVICES

	Devices represent a serial device or a TCP/IP host for busybeed(8) to
	communicate with.  busybeed(8) will setup sockets and bind to devices
	depending on the settings for each device.

baud number

	Set the baud rate for a serial device location.

bind interface string

	Specify a single interface to create a location or ipaddr device
	socket.

data number

	Set the data bits for a serial device location.

connect on port number

	Set the TCP/IP port to connect to for an ipaddr device. This
	setting is required for ipaddr devices.

device string {...}

	Declaration of a device for busybeed(8) to setup and the name
	busybeed(8) will use for client subscriptions.

hardware number

	Set to 1 to turn on hardware control for a serial device
	location.  OpenBSD doesn't use this. It's commented out in
	busybeed(8).

ipaddr string {...}

	Create a device connection to an IPV4 hostname or IP address.
	IPV6 is not implemented yet.

listen on port number

	Set a local socket port for a device location or ipaddr other
	than the global default port.

location string {...}

	Create a device connection to serial interface. These are any
	valid /dev devices, for example /dev/ttyU0.

max clients number

	Set the maximum clients allowed to connect to a device location
	or ipaddr, other than the global max client value.

parity string

	Set the parity for a serial device location.  Parity options are
	none, odd, even, or space.

password string

	Set the subscription password for a device location or ipaddr.

persistent number

	Set whether busybeed(8) binds to an ipaddr device. If persistent
	is set to 0, busybeed(8) will create a socket, send a client
	packet, and close that socket. Clients cannot receive on a non-
	persistent ipaddr device.

software number

	Set to 1 to turn on software control for a serial device
	location.

stop number

	Set the stop bit for a serial device location.

FILES

	/etc/busybeed.conf
	busybeed(8) configuration file.

EXAMPLES

	This configuration file would create a serial device and a TCP/IP host
	device.  The serial device would communicate on the default port, while
	the local network ipaddr device would communicate on a separate port.

	#
	# Global Options
	#

	default port 50000

	max clients 10

	max suscriptions 5

	device "serial_device" {
		location "/dev/ttyU0" {
			baud 9600
			max clients 1
			data 8
			parity "none"
			stop 1
			#no password required for our one allowed client
		}
	}

	device "ipaddr_device" {
		ipaddr "10.0.0.1" {
			connect on port 8080
			listen on port 50010
			password "password"
		}
	}

SEE ALSO

	busybctl(8), busybeed(8)

HISTORY

	The busybeed.conf file format first appeared with busybeed developed on
	OpenBSD 5.9.

AUTHORS

	The busybeed(8) program was written by Tracey Emery
	<tracey@traceyemery.com>, with extensive reading and perusing of ntpd(8)
	and relayd(8).  Thank you to the real programmers who came before me.

CAVEATS

	busybeed(8) None yet.

---------------------------------------------------

NAME

	busybctl - busybeed daemon controller

DESCRIPTION

	Not yet developed.
