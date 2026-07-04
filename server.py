import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("127.0.0.1", 12345))

print("Listening...")

while True:
    data, addr = sock.recvfrom(1024)

    print("Received:", data)
    print("From:", addr)

    sock.sendto(b"Hello from server\n", addr)