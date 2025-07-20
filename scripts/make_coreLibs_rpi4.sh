# libgcc.a libc.a libm.a libnosys.a libstdc++.a
# sdcard.a tftpserver.a fatfs.a input.a usb.a net.a sched.a sound.a
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
aarch64-none-elf-ar -q -c --thin libCore.a \
    libsysPlatform.a \
    libglobalCompat.a \
    libAvalon.a \
    libStride.a \
    libcircle.a \
    libCMSISDSP.a \
    libfatfs.a \
    libfs.a \
    libinput.a \
    libnet.a \
    libsched.a \
    libsdcard.a \
    libsound.a \
    libtftpfileserver.a \
    libusb.a \
&& echo -e 'create libCore.a\naddlib libCore.a\nsave\nend' | aarch64-none-elf-ar -M

mv libCore.a core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
zip coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip core.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.dat
mv coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip ../
cd .. && ln -s coreLibs.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip coreLibs.zip
cd ${cwd}

# Now create the includeFull.zip
cd ../dist/${ARCH}/include
rm -rf ../includeFull.*.zip
zip -r ../includeFull.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip \
    sysPlatform globalCompat Avalon Stride circle SDCard tftpfileserver arm_math
cd .. && ln -s includeFull.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip includeFull.zip
cd ${cwd}

# Now create the includeEfxDev.zip
cd ../dist/${ARCH}/include
rm -rf ../includeEfxDev*.zip
zip -r ../includeEfxDev.${LIB_MAJOR_VER}.${LIB_MINOR_VER}.${LIB_PATCH_VER}.zip globalCompat sysPlatform Avalon Stride arm_math
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

