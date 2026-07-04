#include "include/protocol.h"

int parse_ipv4(uint8_t *buf, ipv4_hdr *ip)
{
    ip->version = buf[0] >> 4;
    ip->ihl = buf[0] & 0x0f;
    ip->tos = buf[1];
    memcpy(&ip->total_len, buf + 2, 2);
    ip->total_len = ntohs(ip->total_len);
    memcpy(&ip->id, buf + 4, 2);
    ip->id = ntohs(ip->id);
    uint16_t frag;
    memcpy(&frag, buf + 6, 2);
    frag = ntohs(frag);
    ip->df = (frag >> 14) & 1;
    ip->mf = (frag >> 13) & 1;
    ip->frag_offset = frag & 0x1FFF;
    ip->ttl = buf[8];
    ip->proto = buf[9];
    memcpy(&ip->checksum, buf + 10, 2);
    ip->checksum = ntohs(ip->checksum);
    memcpy(&ip->src, buf + 12, 4);
    memcpy(&ip->dest, buf + 16, 4);
    return ip->ihl * 4;
}

ipv4_raw reply_ip(uint8_t *reply, ipv4_hdr *ip, uint16_t protocol, uint16_t payload_len,int *pos)
{
    uint16_t flag = htons(0);
    uint16_t proto = htons(0x0800);
    
    memcpy(reply, &flag, 2);
    *pos += 2;
    memcpy(reply + *pos, &proto, 2);
    *pos += 2;
    ipv4_raw ipv4;
    ipv4.ver_ihl = 0x45;
    ipv4.tos = 0;
    ipv4.total_len = htons(20 + payload_len);
    ipv4.id = htons(1);

    ipv4.frag = htons(0);

    ipv4.ttl = 64;

    ipv4.proto = protocol;

    ipv4.checksum = 0;
    ipv4.src = ip->dest;
    ipv4.dest = ip->src;
    memcpy(reply + *pos, &ipv4, sizeof(ipv4));
    *pos += sizeof(ipv4);

    return ipv4;
}
