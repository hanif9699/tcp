#include "include/protocol.h"

int parse_udp(uint8_t *buf, udp_hdr *udp)
{
    memcpy(&udp->src_port, buf, 2);
    udp->src_port = ntohs(udp->src_port);
    memcpy(&udp->dest_port, buf + 2, 2);
    udp->dest_port = ntohs(udp->dest_port);
    memcpy(&udp->length, buf + 4, 2);
    udp->length = ntohs(udp->length);
    memcpy(&udp->checksum, buf + 6, 2);
    udp->checksum = ntohs(udp->checksum);
    return 8;
}

void reply_udp(int fd, ipv4_hdr *ip, udp_hdr *udp, uint8_t *payload, uint16_t payload_len)
{
    uint8_t reply[1024];
    ipv4_raw ipv4;
    udp_raw udp4;
    int pos = 0;
    ipv4 = reply_ip(reply, ip, IP_PROTO_UDP, payload_len + 8, &pos);
    udp4.src_port = htons(udp->dest_port);
    udp4.dst_port = htons(udp->src_port);
    udp4.len = htons(8 + payload_len);
    udp4.checksum = 0;
    uint8_t udp_packet[8 + payload_len];

    memcpy(udp_packet, &udp4, sizeof(udp4));
    memcpy(udp_packet + 8, payload, payload_len);

    udp4.checksum = udp_checksum(ipv4.src, ipv4.dest, (uint8_t *)udp_packet, sizeof(udp_packet));
    udp4.checksum = udp4.checksum;
    // udp4.checksum = 0;
    memcpy(reply + pos, &udp4, sizeof(udp4));
    pos += sizeof(udp4);

    memcpy(reply + pos, payload, payload_len);

    pos += payload_len;

    ipv4_raw *iph = (ipv4_raw *)(reply + 4);
    iph->checksum = ipv4_checksum(iph, 20);
    int written = write(fd, reply, pos);
    printf("sent %d bytes\n", written);
}