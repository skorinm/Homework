#!/bin/bash

echo 30 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio30/direction
//echo 1 > /sys/class/gpio/gpio30/value

echo `i2cget -y 1 0x48 0` # 48 = inner
echo `i2cset -y 1 0x48 1 0` # set config options
echo `i2cset -y 1 0x48 2 0x1e` # set low 89F
echo `i2cset -y 1 0x48 3 0x22` # set high 100F
