#!/bin/bash
# Installs the Midnight Mansion HD lobby server as a service on a Linux server (Ubuntu or
# Oracle Linux, e.g. an Oracle Cloud "Always Free" instance), and opens its port in the
# server's own firewall.
#
#   sudo bash install.sh [port]            (default port 47480)
#
# Run it from the folder that holds mmhd_lobby.py. Running it again updates the server.
# The cloud provider's firewall (Oracle: the VCN's security list) must allow the port too.
set -euo pipefail

PORT="${1:-47480}"
HERE="$(cd "$(dirname "$0")" && pwd)"

if [ "$(id -u)" -ne 0 ]; then
    echo "Run it with sudo: sudo bash install.sh" >&2
    exit 1
fi
if [ ! -f "$HERE/mmhd_lobby.py" ]; then
    echo "mmhd_lobby.py isn't next to install.sh" >&2
    exit 1
fi
if ! command -v python3 >/dev/null; then
    if command -v apt-get >/dev/null; then apt-get update -q && apt-get install -y -q python3
    else dnf install -y python3; fi
fi

install -d /opt/mmhd
install -m 0644 "$HERE/mmhd_lobby.py" /opt/mmhd/mmhd_lobby.py
if [ -f "$HERE/mmhd_admin.py" ]; then
    install -m 0644 "$HERE/mmhd_admin.py" /opt/mmhd/mmhd_admin.py
    printf '#!/bin/sh\nexec python3 /opt/mmhd/mmhd_admin.py "$@"\n' > /usr/local/bin/mmhd-admin
    chmod 0755 /usr/local/bin/mmhd-admin
fi

cat > /etc/systemd/system/mmhd-lobby.service <<EOF
[Unit]
Description=Midnight Mansion HD lobby server
After=network-online.target
Wants=network-online.target
StartLimitIntervalSec=0

[Service]
ExecStart=$(command -v python3) /opt/mmhd/mmhd_lobby.py --port $PORT --state /var/lib/mmhd-lobby --admin-socket /run/mmhd-lobby/admin.sock
# bans, settings and the VPN lists (kept across restarts); the socket mmhd-admin talks to
StateDirectory=mmhd-lobby
RuntimeDirectory=mmhd-lobby
Restart=always
RestartSec=3
LimitNOFILE=8192
DynamicUser=yes
NoNewPrivileges=yes
ProtectSystem=strict
ProtectHome=yes
PrivateTmp=yes

[Install]
WantedBy=multi-user.target
EOF
systemctl daemon-reload
systemctl enable --now mmhd-lobby
systemctl restart mmhd-lobby

# The server's own firewall. Oracle's Ubuntu images reject everything but SSH with iptables
# rules; Oracle Linux uses firewalld; elsewhere ufw may be on.
if command -v firewall-cmd >/dev/null && firewall-cmd --state >/dev/null 2>&1; then
    firewall-cmd --permanent --add-port="$PORT/tcp" >/dev/null
    firewall-cmd --reload >/dev/null
    echo "firewalld: port $PORT/tcp open"
elif command -v ufw >/dev/null && ufw status | grep -q "Status: active"; then
    ufw allow "$PORT/tcp" >/dev/null
    echo "ufw: port $PORT/tcp open"
elif command -v iptables >/dev/null; then
    if ! iptables -C INPUT -p tcp --dport "$PORT" -m conntrack --ctstate NEW -j ACCEPT 2>/dev/null; then
        iptables -I INPUT -p tcp --dport "$PORT" -m conntrack --ctstate NEW -j ACCEPT
    fi
    if command -v netfilter-persistent >/dev/null; then
        netfilter-persistent save >/dev/null 2>&1 || true
    elif [ -d /etc/iptables ]; then
        iptables-save > /etc/iptables/rules.v4
    fi
    echo "iptables: port $PORT/tcp open"
fi

sleep 1
if systemctl is-active --quiet mmhd-lobby; then
    echo
    echo "The lobby server is running on port $PORT."
    echo "Look after it (who's on, disconnect, ban, VPN blocking):  mmhd-admin"
    echo "Logs:    journalctl -u mmhd-lobby -f"
    echo "Restart: sudo systemctl restart mmhd-lobby"
else
    echo "The service didn't start; see: journalctl -u mmhd-lobby -n 50" >&2
    exit 1
fi
