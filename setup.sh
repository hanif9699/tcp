#!/bin/sh

sudo ip addr add 192.168.0.1/24 dev tun0
sudo ip link set tun0 up