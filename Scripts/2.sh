sudo sysctl -w net.ipv4.ip_forward=1
sudo ip route add 10.4.13.5 via 10.4.13.1 dev eth0
sudo ip route add 10.4.13.4 via 10.4.13.3 dev eth0
sudo ip route add 10.4.13.6 via 10.4.13.3 dev eth0