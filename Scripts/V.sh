sudo ip route flush cache
sudo sysctl -w net.ipv4.conf.all.accept_redirects=0
sudo ip route add 10.4.13.1 via 10.4.13.6 dev eth0
