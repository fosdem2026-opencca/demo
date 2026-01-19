#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


set -x
cd $SCRIPT_DIR/../../linux/drivers/gpu/drm/vkms
insmod ./vkms.ko || true

mkdir -p /etc/X11/xorg.conf.d
tee /etc/X11/xorg.conf.d/20-vkms.conf >/dev/null <<'EOF'
Section "Device"
  Identifier "VKMS"
  Driver "modesetting"
  Option "kmsdev" "/dev/dri/card1"
  Option "DRI" "3"
EndSection

Section "Screen"
  Identifier "Screen0"
  Device "VKMS"
EndSection
EOF


export DISPLAY=:1
pkill -f xorg || true

rm -rf /tmp/.X1* || true
pkill -f tigervncserver &> /dev/null || true


mkdir -p ~/.config/openbox ~/.config/tigervnc ~/.vnc
rm -rf ~/.fehbg || true


cat > ~/.Xresources <<'EOF'
XTerm*faceName: DejaVu Sans Mono
XTerm*faceSize: 16
EOF


cat > ~/.vnc/xstartup <<EOF
#!/bin/sh

unset SESSION_MANAGER
unset DBUS_SESSION_BUS_ADDRESS
export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin


[ -f "$HOME/.Xresources" ] && xrdb -merge "$HOME/.Xresources"

fluxbox &

xterm  -T "PANTHOR" -e sh -lc '
tmux kill-server 2>/dev/null || true
tmux new-session -d -s panthor "watch -d -n 1 '\''grep -i panthor /proc/interrupts'\''"
tmux split-window -v "bash"
tmux select-pane -t 1
tmux attach -t panthor
' &

pkill -f fosdem
"${SCRIPT_DIR}/fosdem" &

wait
EOF
chmod +x ~/.vnc/xstartup

export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
/usr/bin/tigervncserver \
  -geometry  1600x900 -depth 24 \
  -rfbport 5901 \
  -localhost=0 \
  -SecurityTypes None \
  --I-KNOW-THIS-IS-INSECURE \
   -xstartup ~/.vnc/xstartup \
  -verbose 

# -fg  

