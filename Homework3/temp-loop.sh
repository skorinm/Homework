#!/bin/bash

# Running this program will spit out the temp of both temp sensors mounted on the board.
# 9/19/13 Matt Skorina

clear

echo 51 > /sys/class/gpio/export
echo in > /sys/class/gpio/gpio51/direction

while true; do
	temp_48=`i2cget -y 1 0x48 0`
	temp_49=`i2cget -y 1 0x49 0`

	temp_48=`echo $((temp_48))*1.8+32 | bc`
	echo "Inner sensor is" $temp_48 "F"

	temp_49=`echo $((temp_49))*1.8+32 | bc`
	echo "Outer sensor is" $temp_49 "F"
	echo Inner Sensor ALERT=`cat /sys/class/gpio/gpio51/value`
	sleep 1
	
	clear
done
