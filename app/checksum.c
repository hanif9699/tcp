#include "include/protocol.h"

uint16_t ipv4_checksum(void *data, int len)
{
    uint32_t sum = 0;
    uint16_t *ptr = (uint16_t *)data;
    while (len > 1)
    {
        sum += *ptr++;
        len -= 2;
    }
    if (len)
    {
        sum += *((uint8_t *)ptr);
        len--;
    }
    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)~sum;
}

uint16_t udp_checksum(uint32_t src_ip, uint32_t dest_ip, uint8_t *udp_packet, uint16_t udp_len)
{
    udp_pseudo_hdr pseudo;
    pseudo.src_ip = src_ip;
    pseudo.dst_ip = dest_ip;
    pseudo.zero = 0;
    pseudo.protocol = 17;
    pseudo.udp_len = htons(udp_len);

    uint16_t total_len = sizeof(pseudo) + udp_len;
    uint8_t *buf = calloc(1, total_len);

    memcpy(buf, &pseudo, sizeof(pseudo));
    memcpy(buf + sizeof(pseudo), udp_packet, udp_len);
    printf("UDP packet (%d bytes):\n", total_len);

    for (int i = 0; i < total_len; i++)
    {
        printf("%02x ", buf[i]);
    }
    // printf("\n");
    uint16_t checksum = ipv4_checksum(buf, total_len);
    printf("%02x", checksum);
    free(buf);
    return checksum;
}

uint16_t tcp_checksum(uint32_t src_ip, uint32_t dst_ip, uint8_t *tcp_segment, uint16_t tcp_len)
{
    tcp_pseudo_hdr pseudo;

    pseudo.src_ip = src_ip;
    pseudo.dst_ip = dst_ip;
    pseudo.zero = 0;
    pseudo.protocol = 6;
    pseudo.tcp_len = htons(tcp_len);

    uint16_t total_len = sizeof(pseudo) + tcp_len;

    uint8_t *buf = calloc(1, total_len);

    memcpy(buf, &pseudo, sizeof(pseudo));
    memcpy(buf + sizeof(pseudo), tcp_segment, tcp_len);

    uint16_t sum = ipv4_checksum(buf, total_len);

    free(buf);

    return sum;
}
