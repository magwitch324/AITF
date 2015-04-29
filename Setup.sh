sudo apt-get update
sudo apt-get install -y libboost-all-dev make g++ pkg-config libssl-dev libnetfilter-queue-dev

cd Libraries/libmnl-1.0.3/
sudo chmod +x ./configure
./configure
make
sudo make install

cd ../libnfnetlink-1.0.1/
sudo chmod +x ./configure
./configure
make
sudo make install


cd ../libnetfilter_queue-1.0.2/
sudo chmod +x ./configure
./configure
make
sudo make install

cd ../../Gateway
make