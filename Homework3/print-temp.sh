#!/bin/bash

temp_48=`i2cget -y 1 0x48 0`
temp_48=`echo $((temp_48))*1.8+32 | bc`
echo "Inner sensor is" $temp_48 "F"