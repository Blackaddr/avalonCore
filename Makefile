# Library version.  Must use Semantic Versioning 'x.y.z'.  E.g. '1.5.2'
APP_MAJOR_VER ?= 0
APP_MINOR_VER ?= 0
APP_PATCH_VER ?= 0
APP_VER = $(APP_MAJOR_VER).$(APP_MINOR_VER).$(APP_PATCH_VER)

# Name of the library (do not prefix with 'lib'), e.g. MyUtil
TARGET_NAME = AvalonCore

DOC_PATH_DOXYGEN =

# List of source files (do not include extentions.  E.g. 'MyClass', not 'MyClass.cpp')
#CPP_SRC_LIST = $(subst /$(TARGET_NAME)/,/, $(subst .cpp,,$(wildcard ./$(TARGET_NAME)/*.cpp)))
#CPP_SRC_LIST += $(subst /$(TARGET_NAME)/,/, $(subst .cpp,,$(wildcard ./$(TARGET_NAME)/utility/*.cpp)))
CPP_SRC_LIST += AvalonCore \
    Tremolo TremoloBase \
    PlateReverb PlateReverbBase \
    AnalogDelay AnalogDelayBase \
    IrData

C_SRC_LIST =
S_SRC_LIST =

# List of Git-repo dependencies
DEP_BUILD_LIST = \
    Blackaddr/libGlobalCompat.git \
    Blackaddr/libStride.git \
    Blackaddr/libAvalon.git

# List of system include/lib directories not checked by default by your compiler.
# Always use the -I=<dir> and -L=<dir> syntax, not -I<dir>, etc.
#-Isrc/i2c/$(TARGET_OS) - is needed to include the correct i2cOs.h
SYS_INC_DIRS  =
SYS_LIBS_DIRS =

# List of libraries to be compiled and linked dynamically. This is required for LGPL licenced libraries. Optional for others.
# example: SYS_DYN_LIBS_LIST = GLEW glfw GL freetype X11 Xxf86vm Xrandr pthread Xi jpeg tiff avcodecm
SYS_DYN_LIBS_LIST =

# List of libraries to be compiled and linked statically. This can only be used for BSD and other similar permissively-licensed software
# example: SYS_STAT_LIBS_LIST = SysUtil Frame
SYS_STAT_LIBS_LIST = \
    Stride \
    Avalon \
    globalCompat sysPlatform

PREPROC_DEFINES_LIST =

MAKEFILE_NAME ?= Makefile

# Check if build engine location has already been defined
export APP_BUILD_ENGINE_LOC ?= $(CURDIR)/scripts/avalon_app_build_engine.$(MCU_TYPE)/
include $(APP_BUILD_ENGINE_LOC)/Makefile.inc

