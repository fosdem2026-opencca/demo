#!/usr/bin/env bash
set -euo pipefail

INTF="${INTF:-enP4p65s0}"

DEMO_IP="${DEMO_IP:-10.42.0.30/24}"
NORMAL_IP="${NORMAL_IP:-192.33.93.163/24}"

DIR="/etc/systemd/network"
DEMO_SRC="$DIR/fosdem-demo.network"
NORMAL_SRC="$DIR/fosdem-normal.network"

DEMO_LINK="$DIR/10-active.network"
NORMAL_LINK="$DIR/10-active.network"

write_src_files() {
  sudo tee "$DEMO_SRC" >/dev/null <<EON
[Match]
Name=$INTF

[Network]
Address=$DEMO_IP
LinkLocalAddressing=no
IPv6AcceptRA=no
EON

  sudo tee "$NORMAL_SRC" >/dev/null <<EON
[Match]
Name=$INTF

[Network]
Address=$NORMAL_IP
LinkLocalAddressing=no
IPv6AcceptRA=no
EON
}

apply() {
  local target="$1"
  write_src_files

  if [[ "$target" == "demo" ]]; then
    echo "[*] Applying DEMO on $INTF: $DEMO_IP"
    sudo ln -sf "$(basename "$DEMO_SRC")" "$DEMO_LINK"
  else
    echo "[*] Applying NORMAL on $INTF: $NORMAL_IP"
    sudo ln -sf "$(basename "$NORMAL_SRC")" "$NORMAL_LINK"
  fi

  sudo systemctl restart systemd-networkd

  echo "[*] Interface:"
  ip -br link show dev "$INTF" || true
  ip -br addr show dev "$INTF" || true

  echo "[*] Routes on $INTF:"
  ip route show dev "$INTF" || true

  echo "[+] Done."
}

status() {
  echo "[*] systemd-networkd:"
  systemctl is-active systemd-networkd || true
  echo
  echo "[*] Active network file link:"
  ls -l "$DIR/10-active.network" 2>/dev/null || echo "no active link"
  echo
  echo "[*] Interface:"
  ip -br link show dev "$INTF" || true
  ip -br addr show dev "$INTF" || true
  echo
  echo "[*] Routes:"
  ip route | sed -n '1,80p'
}

case "${1:-}" in
  up) apply demo ;;
  down) apply normal ;;
  status) status ;;
  *)
    echo "Usage: $0 {up|down|status}"
    echo "Env: INTF=$INTF DEMO_IP=$DEMO_IP NORMAL_IP=$NORMAL_IP"
    exit 1
    ;;
esac
