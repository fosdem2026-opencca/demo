#!/bin/bash

sudo ip route del 192.33.93.160/32 2>/dev/null || true
sudo ip route del 192.33.93.163/32 2>/dev/null || true
sudo ip addr del 192.33.93.250/24 dev enp0s25 2>/dev/null || true