#!/bin/bash

sudo ip addr add 192.33.93.250/24 dev enp0s25 2>/dev/null || true
sudo ip link set enp0s25 up

# Force routes to the demo targets via that interface (prevents Wi-Fi/VPN confusion)
sudo ip route replace 192.33.93.160/32 dev enp0s25
sudo ip route replace 192.33.93.163/32 dev enp0s25