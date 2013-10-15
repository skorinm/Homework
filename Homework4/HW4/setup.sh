#/bin/bash
# Setup the bone to read from the analog pins.

SLOTS=/sys/devices/bone_capemgr.8/slots #8 should be * but working :(
PINS=/sys/kernel/debu/pinctl/44e10800.pinmux/pins
#if ! [ -a $SLOTS ] # If file does not exist
#then
echo cape-bone-iio > $SLOTS
#fi
