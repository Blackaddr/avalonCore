# Adafruit_SSD1306 Adafruit-GFX-Library Adafruit_BusIO OpenAudio Audio Bounce2 Encoder SPI Wire cores
if [ -z "${LIB_MAJOR_VER}" ]; then
export LIB_MAJOR_VER=0
fi

if [ -z "${LIB_MINOR_VER}" ]; then
export LIB_MINOR_VER=0
fi

if [ -z "${LIB_PATCH_VER}" ]; then
export LIB_PATCH_VER=0
fi


cwd=${PWD}
cd ../dist/${ARCH}/lib
rm -f libCore*.a
arm-none-eabi-gcc-ar -q -cT libCore.a \
       libsysPlatform.a \
       libglobalCompat.a \
       libAvalon.a \
       libStride.a \
       libAdafruit_SSD1306.a \
       libAdafruit_SH1106.a \
       libAdafruit-GFX-Library.a \
       libAdafruit_BusIO.a \
       libBounce2.a \
       libEncoder.a \
       libTeensy-4.x-Quad-Encoder-Library.a \
       libSPI.a \
       libWire.a \
       libMIDI.a \
       libTeensyThreads.a \
       libWDT_T4.a \
       libEEPROM.a \
       libLittleFS.a \
       libcores.a \
&& echo -e 'create libCore.a\naddlib libCore.a\nsave\nend' | arm-none-eabi-gcc-ar -M
# NOTE: the ranlib packaged with ARM64 tools doesn't work so we must use the host x86 version
#ranlib libCore.a
mv libCore.a core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
zip coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
mv coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip ../
## Make a separate MacOS package because ranlib is broken on Mac
#ranlib core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
#zip coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.macos.zip core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
cd .. && ln -s coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip coreLibs.zip
cd ${cwd}

# Now create the includeFull.zip
cd ../dist/${ARCH}/include
rm -rf ../includeFull.*.zip
zip -r ../includeFull.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip sysPlatform globalCompat Adafruit_BusIO Adafruit-GFX-Library Adafruit_SSD1306 Adafruit_SH1106 Avalon Stride Bounce2 cores Encoder Teensy-4.x-Quad-Encoder-Library MIDI SPI TeensyThreads Wire WDT_T4 EEPROM LittleFS
cd .. && ln -s includeFull.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip includeFull.zip
cd ${cwd}

# Now create the includeEfxDev.zip
cd ../dist/${ARCH}/include
rm -rf ../includeEfxDev*.zip
zip -r ../includeEfxDev.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip globalCompat sysPlatform Avalon Stride cores SPI TeensyThreads
cd .. && ln -s includeEfxDev.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip includeEfxDev.zip
cd ${cwd}

# Create the CoreVersion.h
echo -e "\
#pragma once\n\
\n\
#define CORELIB_MAJOR_VER ${LIB_MAJOR_VER}\n\
#define CORELIB_MINOR_VER ${LIB_MINOR_VER}\n\
#define CORELIB_PATCH_VER ${LIB_PATCH_VER}\n\
" > ../dist/${ARCH}/CoreVersion.h

