#include "include/protocol.h"

int parse_tun(uint8_t *buf, tun_hdr *tun)
{
    memcpy(&tun->flag, buf, 2);
    memcpy(&tun->proto, buf + 2, 2);
    tun->flag = ntohs(tun->flag);
    tun->proto = ntohs(tun->proto);
    return 4;
}