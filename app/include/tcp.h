#ifndef TCP_H
#define TCP_H
#include<stdint.h>
typedef struct
{
    uint16_t src_port;
    uint16_t dest_port;
    uint32_t seq_no;
    uint32_t ack_no;
    uint8_t d_offset;
    uint8_t res;
    uint8_t ns;
    uint8_t cwr;
    uint8_t ece;
    uint8_t urg;
    uint8_t ack;
    uint8_t psh;
    uint8_t rst;
    uint8_t syn;
    uint8_t fin;
    uint16_t windows_size;
    uint16_t checksum;
    uint16_t urg_ptr;
} tcp_hdr;

typedef struct __attribute__((packed))
{
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint32_t ack;

    uint8_t data_offset;
    uint8_t flags;

    uint16_t window;
    uint16_t checksum;
    uint16_t urg_ptr;
} tcp_raw;

typedef struct __attribute__((packed))
{
    uint32_t src_ip;
    uint32_t dst_ip;
    uint8_t zero;
    uint8_t protocol;
    uint16_t tcp_len;
} tcp_pseudo_hdr;

typedef enum
{
    TCP_CLOSED,
    TCP_SYN_RECEIVED,
    TCP_ESTABLISHED,
    TCP_CLOSE_WAIT
} tcp_state;

typedef struct {
    tcp_state state;
    uint32_t client_ip;
    uint32_t src_ip;
    uint32_t client_port;
    uint32_t src_port;
    uint32_t client_next_seq;
    uint32_t server_next_seq;
} tcp_conn;

int parse_tcp(uint8_t *buf, tcp_hdr *tcp);
void reply_tcp(int fd, ipv4_hdr *ip, tcp_hdr *tcp);

#endif