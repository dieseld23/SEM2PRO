if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

#Find your device name using:
#	ip link show

#choose the wifi device

systemctl stop connman.service
ip link set wlp2s0b1 down
iw wlp2s0b1 set type ibss
ip addr add 196.178.10.9/24 dev wlp2s0b1
ip link set wlp2s0b1 up
iw wlp2s0b1 ibss join go-kart 2412