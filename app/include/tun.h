#ifndef TUN_H
#define TUN_H
#include <stdint.h>
typedef struct
{
    uint16_t flag;
    uint16_t proto;
} tun_hdr;
int parse_tun(uint8_t *buf, tun_hdr *tun);
#endif