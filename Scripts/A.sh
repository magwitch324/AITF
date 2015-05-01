sudo ip route flush cache
sudo sysctl -w net.ipv4.conf.all.accept_redirects=0
sudo ip route add 10.4.13.7 via 10.4.13.2 dev eth0


for i in `seq 127 255`;
	do
		sudo ifconfig eth0:$i 10.4.13.$i down
       	sudo ifconfig eth0:$i 10.4.13.$i up
       	sudo ip route add 10.4.13.7 via 10.4.13.3 dev eth0 src 10.4.13.$i
done    

sudo ip route flush cache
sudo sysctl -w net.ipv4.conf.all.accept_redirects=0
sudo sysctl -w net.ipv4.conf.all.send_redirects=0
sudo iptables -A OUTPUT -p icmp --icmp-type redirect -j DROP
sudo iptables -A INPUT -p icmp --icmp-type redirect -j DROP