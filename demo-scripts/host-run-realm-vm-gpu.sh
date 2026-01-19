#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$SCRIPT_DIR/../../
SNAPSHOT_DIR=$ROOT_DIR/snapshot
BIN=$SNAPSHOT_DIR/lkvm

set -x

KERNEL=$SNAPSHOT_DIR/Image

sudo ip link set tap0 down 2>/dev/null || true
sudo ip tuntap del dev tap0 mode tap 2>/dev/null || true

sudo ip tuntap add dev tap0 mode tap user "$USER"
sudo ip addr add 192.168.33.1/24 dev tap0 2>/dev/null || true
sudo ip link set tap0 up promisc on
ip addr show tap0

sudo sysctl -w net.ipv4.ip_forward=1
WAN=enP4p65s0
LAN=tap0

sudo iptables-legacy -t nat -A POSTROUTING -o "$WAN" -j MASQUERADE
sudo iptables-legacy -A FORWARD -i "$LAN" -o "$WAN" -j ACCEPT
sudo iptables-legacy -A FORWARD -i "$WAN" -o "$LAN" -m conntrack --ctstate ESTABLISHED,RELATED -j ACCEPT

nice -n -20 taskset 1 $BIN run \
  --console virtio \
  -n mode=tap,tapif=tap0,guest_mac=02:aa:bb:cc:dd:01 \
  --realm --restricted_mem \
  -c 1 -m 512 \
  -k "${KERNEL}" \
  --9p "/,host0" \
  --debug \
  -p "nokaslr console=hvc0 ip=192.168.33.2::192.168.33.1:255.255.255.0::eth0:off" \
  --vcpu-affinity 1 \
 --dump-dtb $SCRIPT_DIR/guest.dtb \
 --demo-board-gpu --demo-realm-ns