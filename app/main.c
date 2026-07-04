#include "include/protocol.h"

int main()
{
    struct ifreq ifr;
    int fd;
    fd = open("/dev/net/tun", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = IFF_TUN;
    strcpy(ifr.ifr_name, "tun0");

    if (ioctl(fd, TUNSETIFF, &ifr) < 0)
    {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        return 1;
    }
    printf("Created interface %s \n", ifr.ifr_name);
    while (1)
    {
        uint8_t buf[1504];
        ssize_t n = read(fd, buf, sizeof(buf));
        tun_hdr tun;
        ipv4_hdr ip;
        icmp_hdr icmp;
        tcp_hdr tcp;
        udp_hdr udp;
        int offset = parse_tun(buf, &tun);
        if (tun.proto != (0x0800))
        {
            printf("Skipping tun proto: %02x\n", tun.proto);
            continue;
        }
        printf("Read %zd bytes\n", n);
        printf("flags: %02x, proto: %02x\n", tun.flag, tun.proto);
        offset += parse_ipv4(buf + offset, &ip);
        char srcbuf[INET_ADDRSTRLEN];
        char dstbuf[INET_ADDRSTRLEN];
        char *type;
        struct in_addr src, dest;
        uint16_t payload_len;
        uint8_t *payload;
        switch (ip.proto)
        {
        case IP_PROTO_ICMP:
            offset += parse_icmp(buf + offset, &icmp);

            src.s_addr = ip.src;
            dest.s_addr = ip.dest;

            inet_ntop(AF_INET, &src, srcbuf, sizeof(srcbuf));
            inet_ntop(AF_INET, &dest, dstbuf, sizeof(dstbuf));
            type = "ICMP";
            payload_len = ip.total_len - (ip.ihl * 4);
            printf("%s -> %s | Proto = %s(%d) | Payload = %u bytes", srcbuf, dstbuf, type, ip.proto, payload_len);
            break;
        case IP_PROTO_TCP:
            offset += parse_tcp(buf + offset, &tcp);
            src.s_addr = ip.src;
            dest.s_addr = ip.dest;

            inet_ntop(AF_INET, &src, srcbuf, sizeof(srcbuf));
            inet_ntop(AF_INET, &dest, dstbuf, sizeof(dstbuf));
            type = "TCP";
            payload_len = ip.total_len - (ip.ihl * 4) - tcp.d_offset * 4;
            payload = buf + offset;
            printf("%s -> %s | Proto = %s(%d) | Payload = %u bytes\n", srcbuf, dstbuf, type, ip.proto, payload_len);
            printf("TCP %u -> %u [", tcp.src_port, tcp.dest_port);

            if (tcp.syn)
                printf("SYN ");
            if (tcp.ack)
                printf("ACK ");
            if (tcp.fin)
                printf("FIN ");
            if (tcp.rst)
                printf("RST ");
            if (tcp.psh)
                printf("PSH ");
            printf("]\n");
            printf("Payload: \n");

            for (int i = 0; i < payload_len; i++)
                putchar(payload[i]);

            printf("\n");
            if (tcp.syn && !tcp.ack)
            {

                printf("Received SYN\n");
                printf("Client Port : %u\n", tcp.src_port);
                printf("Server Port : %u\n", tcp.dest_port);
                printf("SEQ         : %u\n", tcp.seq_no);
                reply_tcp(fd, &ip, &tcp);
            }
            if (tcp.ack && !tcp.syn)
            {
                printf("TCP connection established!\n");
            }
            
            break;
        case IP_PROTO_UDP:
            offset += parse_udp(buf + offset, &udp);
            src.s_addr = ip.src;
            dest.s_addr = ip.dest;
            type = "UDP";
            payload_len = (udp.length - 8);
            payload = buf + offset;
            inet_ntop(AF_INET, &src, srcbuf, sizeof(srcbuf));
            inet_ntop(AF_INET, &dest, dstbuf, sizeof(dstbuf));
            printf("%s -> %s | Proto = %s(%d) | Payload = %u bytes\n", srcbuf, dstbuf, type, ip.proto, payload_len);
            printf("UDP %u -> %u ", udp.src_port, udp.dest_port);
            reply_udp(fd, &ip, &udp, payload, payload_len);
            break;

        default:
            printf("Dont support this protocal for now");
            break;
        }
        printf("\n");
    }

    close(fd);
    return 0;
}
