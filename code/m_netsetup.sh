if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

#Find your device name using:
#	ip link show

#choose the wifi device

#systemctl stop connman.service
sudo service network-manager stop
ip link set eth1 down
iw eth1 set type ibss
ip addr add 196.178.10.9/24 dev eth1
ip link set eth1 up
iw eth1 ibss join go-kart 2412
