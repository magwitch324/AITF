sudo ip route flush cache
sudo sysctl -w net.ipv4.conf.all.accept_redirects=0
sudo ip route add 10.4.13.7 via 10.4.13.2 dev eth0
