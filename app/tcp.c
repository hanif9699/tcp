#include "include/protocol.h"

int parse_tcp(uint8_t *buf, tcp_hdr *tcp)
{
    memcpy(&tcp->src_port, buf, 2);
    tcp->src_port = ntohs(tcp->src_port);
    memcpy(&tcp->dest_port, buf + 2, 2);
    tcp->dest_port = ntohs(tcp->dest_port);
    memcpy(&tcp->seq_no, buf + 4, 4);
    tcp->seq_no = ntohl(tcp->seq_no);
    memcpy(&tcp->ack_no, buf + 8, 4);
    tcp->ack_no = ntohl(tcp->ack_no);
    uint8_t data = buf[12];
    tcp->d_offset = data >> 4;
    tcp->res = (data & 0x0E) >> 1;
    tcp->ns = data & 0x01;
    data = buf[13];
    tcp->cwr = data >> 7;
    tcp->ece = (data & 0x40) >> 6;
    tcp->urg = (data & 0x20) >> 5;
    tcp->ack = (data & 0x10) >> 4;
    tcp->psh = (data & 0x08) >> 3;
    tcp->rst = (data & 0x04) >> 2;
    tcp->syn = (data & 0x02) >> 1;
    tcp->fin = data & 0x01;
    memcpy(&tcp->windows_size, buf + 14, 2);
    tcp->windows_size = ntohs(tcp->windows_size);
    memcpy(&tcp->checksum, buf + 16, 2);
    tcp->checksum = ntohs(tcp->checksum);
    memcpy(&tcp->urg_ptr, buf + 18, 2);
    tcp->urg_ptr = ntohs(tcp->urg_ptr);
    return tcp->d_offset * 4;
}

void reply_tcp(int fd, ipv4_hdr *ip, tcp_hdr *tcp)
{
    uint8_t reply[1024];
    ipv4_raw ipv4;
    tcp_raw tcp4;
    int pos = 0;
    ipv4 = reply_ip(reply, ip, IP_PROTO_TCP, 20, &pos);

    tcp4.src_port = htons(tcp->dest_port);
    tcp4.dst_port = htons(tcp->src_port);

    tcp4.seq = htonl(1000);
    tcp4.ack = htonl(tcp->seq_no + 1);

    /* Header length = 5 words (20 bytes) */
    tcp4.data_offset = (5 << 4);

    /* SYN + ACK */
    tcp4.flags = 0x12;

    tcp4.window = htons(65535);

    tcp4.checksum = 0;
    tcp4.urg_ptr = 0;

    tcp4.checksum = 0;

    uint8_t tcp_segment[20];
    memcpy(tcp_segment, &tcp4, sizeof(tcp4));

    tcp4.checksum = tcp_checksum(ipv4.src, ipv4.dest, tcp_segment, sizeof(tcp_segment));
    memcpy(reply + pos, &tcp4, sizeof(tcp4));
    pos += sizeof(tcp4);
    ipv4_raw *iph = (ipv4_raw *)(reply + 4);
    iph->checksum = ipv4_checksum(iph, 20);
    int written = write(fd, reply, pos);
    printf("sent %d bytes\n", written);
}
