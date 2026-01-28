#!/usr/bin/env bash
set -euo pipefail

CON_NAME="FOSDEM-LAN"
INTF="enxf8e43b5777dc"
IP_CIDR="10.42.0.10/24"
SUBNET="10.42.0.0/24"

up() {
  echo "[*] Bringing up $CON_NAME on $INTF with $IP_CIDR (local-only)"

  
  if ! nmcli -t -f NAME con show | grep -qx "$CON_NAME"; then
    echo "[*] Creating NetworkManager connection: $CON_NAME"
    sudo nmcli con add type ethernet ifname "$INTF" con-name "$CON_NAME" \
      ipv4.method manual ipv4.addresses "$IP_CIDR" ipv4.never-default yes \
      ipv4.ignore-auto-dns yes ipv4.ignore-auto-routes yes \
      ipv6.method ignore >/dev/null
  else
    echo "[*] Ensuring profile settings are correct"
    sudo nmcli con mod "$CON_NAME" connection.interface-name "$INTF" \
      ipv4.method manual ipv4.addresses "$IP_CIDR" ipv4.never-default yes \
      ipv4.ignore-auto-dns yes ipv4.ignore-auto-routes yes \
      ipv6.method ignore >/dev/null
  fi

  sudo nmcli con up "$CON_NAME" >/dev/null

  echo "[*] Checking link + IP"
  ip -br link show dev "$INTF" || true
  ip -br addr show dev "$INTF" || true

  echo "[*] Checking routes (must NOT add a default route on $INTF)"
  if ip route | grep -q "^default .* dev $INTF"; then
    echo "[!] ERROR: default route is set on $INTF - not local-only!"
    ip route | sed -n '1,40p'
    exit 1
  fi

  if ! ip route | grep -q "^$SUBNET dev $INTF"; then
    echo "[!] WARNING: subnet route $SUBNET not visible (should normally exist)"
    ip route | sed -n '1,80p'
  fi

  echo "[+] OK. Local LAN ready."
  ip addr | grep $INTF
}

down() {
  echo "[*] Bringing down $CON_NAME"
  sudo nmcli con down "$CON_NAME" >/dev/null || true
  echo "[*] Done."
}

status() {
  echo "[*] NM connection status:"
  nmcli -f NAME,UUID,TYPE,DEVICE con show --active | sed -n '1,40p'
  echo
  echo "[*] Interface status:"
  ip -br link show dev "$INTF" || true
  ip -br addr show dev "$INTF" || true
  echo
  echo "[*] Routes:"
  ip route | sed -n '1,80p'
}

case "${1:-}" in
  up) up ;;
  down) down ;;
  status) status ;;
  *)
    echo "Usage: $0 {up|down|status}"
    exit 1
    ;;
esac
