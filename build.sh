#!/bin/bash
MAJOR_VER=1
MINOR_VER=6
PATCH_VER=3

if [ "$ARCH" == "arm" ]; then
  source ./scripts/avalon_app_build_engine.TEENSY_MICROMOD/set_version.sh ${MAJOR_VER} ${MINOR_VER} ${PATCH_VER}
fi
if [ "$ARCH" == "aarch64" ]; then
  source ./scripts/avalon_app_build_engine.RPI4B/set_version.sh ${MAJOR_VER} ${MINOR_VER} ${PATCH_VER}
fi
make distclean && make
cd scripts && source ./make_coreLibs.sh && cd ..

if [ "$MCU_TYPE" == "TEENSY_MICROMOD" ]; then
  binary_file="./dist/${ARCH}/bin/AvalonCore.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.hex"
elif [ "$MCU_TYPE" == "RPI4B" ]; then
  binary_file="./dist/${ARCH}/bin/AvalonCore.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.img"
else
  binary_file="UNSUPPORTED_MCU_TYPE"
  echo "ERROR: MCU_TYPE is not set to a valid value"
  exit 1
fi

if test -f ${binary_file}; then
  echo -e "\nPROGRAM file created successfully: ${binary_file}"
  ./save.sh
else
  echo -e "\nERROR: PROGRAM file does not exist. Please check for compile errors"
fi
