#!/bin/bash

# Running this program will spit out the temp of both temp sensors mounted on the board.
# 9/19/13 Matt Skorina

clear

while true; do
	temp_48=`i2cget -y 1 0x48 0`
	temp_49=`i2cget -y 1 0x49 0`

	temp_48=`echo $((temp_48))*1.8+32 | bc`
	echo "Inner sensor is" $temp_48 "F"

	temp_49=`echo $((temp_49))*1.8+32 | bc`
	echo "Outer sensor is" $temp_49 "F"

	sleep 1
	clear
done
