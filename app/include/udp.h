#ifndef UDP_H
#define UDP_H
#include <stdint.h>
#include "ipv4.h"
typedef struct
{
    uint16_t src_port;
    uint16_t dest_port;
    uint16_t length;
    uint16_t checksum;
} udp_hdr;
typedef struct __attribute__((packed))
{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t len;
    uint16_t checksum;
} udp_raw;

typedef struct __attribute__((packed))
{
    uint32_t src_ip;
    uint32_t dst_ip;
    uint8_t zero;
    uint8_t protocol;
    uint16_t udp_len;
} udp_pseudo_hdr;

int parse_udp(uint8_t *buf, udp_hdr *udp);
void reply_udp(int fd, ipv4_hdr *ip, udp_hdr *udp, uint8_t *payload, uint16_t payload_len);

#endif