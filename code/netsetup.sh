if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

#Find your device name using:
#	ip link show

#choose the wifi device

systemctl stop networkmanager.service
ip link set wlp3s0 down
iw wlp3s0 set type ibss
ip addr add 196.178.10.9/24 dev wlp3s0
ip link set wlp3s0 up
iw wlp3s0 ibss join go-kart 2412
