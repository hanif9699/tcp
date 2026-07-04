#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "tun.h"
#include "ipv4.h"
#include "icmp.h"
#include "tcp.h"
#include "udp.h"
#include "checksum.h"



#define IP_PROTO_ICMP 1
#define IP_PROTO_TCP  6
#define IP_PROTO_UDP 17

#endif