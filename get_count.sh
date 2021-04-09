#!/bin/bash
#sudo btmgmt find |grep rssi |sort -n |uniq -w 33
#echo -e 'power on\nquit' | bluetoothctl >&2
#sleep 5
#hciconfig hci0 up >&2
#sleep 5
# do this for normal work 
#mknod -m 666 /dev/rfcomm0 c 216 0

rfcomm -r connect hci0  20:17:07:25:29:19 &
sleep 7
pid=`pidof rfcomm`

if [ -n "$pid" ]; then 
echo RFCOMM seems connected. rfcomm PID is $pid:

port=/dev/rfcomm0
file=/opt/iCounters/data.txt
echo "Updated: `date +%d.%m.%Y` at `date +%H:%M:%S`" > $file
echo -e -n "READ" > $port
grep -m 2 'Hot\|Cold' < $port >> $file

kill -SIGINT $pid >&2
else
   echo "RFCOMM didn't connect. Exiting..."
   exit 0;
fi

#rfcomm release /dev/rfcomm0 >&2
#echo -e 'power off\nquit' | bluetoothctl >&2

#grep -m 2 'Hot\|Cold' < $port >&2 && exit 0
#cat  $port >&2 && exit 0