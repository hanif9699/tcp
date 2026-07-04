#include "include/protocol.h"

int parse_icmp(uint8_t *buf, icmp_hdr *icmp)
{
    icmp->type = buf[0];
    icmp->code = buf[1];

    memcpy(&icmp->checksum, buf + 2, 2);
    icmp->checksum = ntohs(icmp->checksum);

    memcpy(&icmp->id, buf + 4, 2);
    icmp->id = ntohs(icmp->id);

    memcpy(&icmp->sequence, buf + 6, 2);
    icmp->sequence = ntohs(icmp->sequence);

    return 8;
}