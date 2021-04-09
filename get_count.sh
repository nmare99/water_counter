#!/bin/bash

# Where 20:17:07:25:29:19 - HC-05 MAC address  
rfcomm -r connect hci0  20:17:07:25:29:19 &
sleep 7
pid=`pidof rfcomm`

if [ -n "$pid" ]; then 
echo RFCOMM seems connected. rfcomm PID is $pid:

port=/dev/rfcomm0
# path and filename to store data
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
