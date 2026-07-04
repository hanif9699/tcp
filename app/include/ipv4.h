#ifndef IPV4_H
#define IPV4_H
#include <stdint.h>
typedef struct
{
    uint8_t version;
    uint8_t ihl;
    uint8_t tos;
    uint16_t total_len;
    uint16_t id;
    uint8_t df;
    uint8_t mf;
    uint16_t frag_offset;
    uint8_t ttl;
    uint8_t proto;
    uint16_t checksum;
    uint32_t src;
    uint32_t dest;
} ipv4_hdr;

typedef struct __attribute__((packed))
{
    uint8_t ver_ihl;
    uint8_t tos;
    uint16_t total_len;
    uint16_t id;
    uint16_t frag;
    uint8_t ttl;
    uint8_t proto;
    uint16_t checksum;
    uint32_t src;
    uint32_t dest;
} ipv4_raw;

int parse_ipv4(uint8_t *buf, ipv4_hdr *ip);
ipv4_raw reply_ip(uint8_t *reply, ipv4_hdr *ip, uint16_t protocol, uint16_t payload_len, int *pos);

#endif