#ifndef CHECKSUM_H
#define CHECKSUM_H
#include "ipv4.h"
#include "udp.h"

uint16_t ipv4_checksum(void *data, int len);
uint16_t udp_checksum(uint32_t src_ip, uint32_t dest_ip, uint8_t *udp_packet, uint16_t udp_len);
uint16_t tcp_checksum(uint32_t src_ip, uint32_t dst_ip, uint8_t *tcp_segment, uint16_t tcp_len);

#endif