sudo sysctl -w net.ipv4.ip_forward=1

sudo ip route add 10.4.13.1 via 10.4.13.4 dev eth0
sudo ip route add 10.4.13.2 via 10.4.13.4 dev eth0
sudo ip route add 10.4.13.3 via 10.4.13.4 dev eth0
sudo ip route add 10.4.13.7 via 10.4.13.6 dev eth0