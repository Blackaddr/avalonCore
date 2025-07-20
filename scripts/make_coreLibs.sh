#!/bin/bash
if [ "$MCU_TYPE" == "RPI4B" ]; then
./make_coreLibs_rpi4.sh
elif [ "$MCU_TYPE" == "TEENSY_MICROMOD" ]; then
./make_coreLibs_teensy.sh
else
echo "ERROR: MCU_TYPE environment variable is not set"
fi


