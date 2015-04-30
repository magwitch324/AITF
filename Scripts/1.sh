sudo sysctl -w net.ipv4.ip_forward=1
sudo sysctl -w net.ipv4.conf.all.accept_redirects=0
sudo ip route add 10.4.13.3 via 10.4.13.2 dev eth0
sudo ip route add 10.4.13.4 via 10.4.13.2 dev eth0
sudo ip route add 10.4.13.6 via 10.4.13.2 dev eth0