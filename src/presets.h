#include <cmath>

#include "sysPlatform/SysTypes.h"
#include "sysPlatform/SysDebugPrint.h"
#include "sysPlatform/SysCrashReport.h"
#include "Avalon/AudioConnectionPanel.h"
#include "Avalon/AudioEffectMeasure.h"
#include "Stride/SramManager.h"
#include "Stride/ReportManager.h"
#include "Stride/TunerManager.h"
#include "Avalon/AvalonHardwareControls.h"
#include "Avalon/PresetManager.h"
#include "Avalon/MidiManager.h"
#include "Avalon/FlashManager.h"
#include "Avalon/AudioEffectOutputLevel.h"
#include "Avalon/MemoryUtils.h"
#include "Avalon/EepromLayout.h"
#include "Avalon/Version.h"
#include "Stride/Compatability.h"
#include "Stride/IrLoader.h"
#include "sysPlatform/SysPlatform.h"

using namespace SysPlatform;
using namespace Avalon;
using namespace Stride;

#define NAN_F std::nanf("0")

constexpr unsigned NUM_GLOBAL_INSTANCES = 4;
constexpr unsigned NUM_AUDIO_IO_INSTANCES = 2;
constexpr unsigned NUM_PRESETS = 1;
constexpr unsigned NUM_PANEL_INPUTS = 5;
constexpr unsigned NUM_PANEL_OUTPUTS = 5;
constexpr unsigned MAX_CONTROLS_PER_EFFECT = 9;
constexpr unsigned MAX_EFFECTS_PER_PRESET = 4;
constexpr unsigned MAX_CHARS_PER_EFFECT_CONTROL_NAME = 16;
constexpr unsigned MAX_NUM_MIDI_ASSIGNS  = 0;
constexpr unsigned MAX_NUM_MIDI_ROUTES  = 0;
constexpr unsigned NUM_HARDWARE_ENCODERS = 4;
constexpr unsigned NUM_HARDWARE_SWITCHES = 6;
constexpr unsigned NUM_HARDWARE_LEDS     = 2;
constexpr unsigned NUM_HARDWARE_EXPS     = 2;

#define PRESET0 {3,4,0,2,-1,}

#define PRESET_STREAMCONV0 {0,0,0,0,0,}

int8_t connectionsArray[NUM_PRESETS][NUM_PANEL_OUTPUTS] = {
    PRESET0,
};

int8_t streamConvertArray[NUM_PRESETS][NUM_PANEL_OUTPUTS] = {
    PRESET_STREAMCONV0,
};

uint32_t PRESET_CHECKSUMS[] = {0xB139A1C1, };


float presetControlValues[NUM_PRESETS][MAX_EFFECTS_PER_PRESET][MAX_CONTROLS_PER_EFFECT];

const float PROGMEM flashPresetControlValues[NUM_PRESETS][MAX_EFFECTS_PER_PRESET][MAX_CONTROLS_PER_EFFECT] = {
    {
        {1, 0, 2.2, 5, 8.2, NAN_F,NAN_F,NAN_F,NAN_F, },
        {1, 5, 3, 5, 0, 1, 2, 8, 0,  },
        {NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F, },
        {NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F,NAN_F, },
    },

};

const char* const PROGMEM globalInstanceNamesArray[4] = {
    "TREM",
    "RVB",
    "",
    "",
};

const char* const PROGMEM effectControlNamesArray[] = {
    "BYP",    "WAV",    "RATE",    "DEP",    "VOL",    "BYP",    "RVB",    "TONE",    "MIX",    "ECHO",    "LCUT",    "HCUT",    "VOL",    "OMODE",};


const float PROGMEM effectControlSpecsArray[] = {
    0,0.000000,1.000000,1.000000,    2,0.000000,4.000000,1.000000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    0,0.000000,1.000000,1.000000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    3,0.000000,10.000000,0.100000,    0,0.000000,1.000000,1.000000,};


const uint16_t PROGMEM effectControlUserArray[] = {
    1,    0,    0,    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,    0,};


const uint8_t PROGMEM effectControlEnumIndexArray[] = {
    0,1,0,5,
};
constexpr size_t EFFECT_CONTROL_ENUM_INDEX_ARRAY_SIZE = 4;


const char* const PROGMEM effectControlEnumNamesArray[] = {
    "Sine",    "Triangle",    "Square",    "Sawtooth",    "Random",
};

int16_t effectControlStartLookup[NUM_GLOBAL_INSTANCES] = { 0, 5, -1, -1,};

int8_t effectControlSizeLookup[NUM_GLOBAL_INSTANCES] = { 5, 9, -1, -1,};

int8_t hardwareEncodersMapArray[NUM_PRESETS][NUM_HARDWARE_ENCODERS][2] = {
    {  {0, 2} , {0, 3} , {1, 3} , {1, 1} , },
};

int8_t hardwareSwitchesMapArray[NUM_PRESETS][NUM_HARDWARE_SWITCHES][2] = {
    {  {0, 0} , {1, 0} , {-1, -1} , {-1, -1} , {-1, -1} , {-1, -1} , },
};

int8_t hardwareLedsMapArray[NUM_PRESETS][NUM_HARDWARE_LEDS][2] = {
    {  {0, 0} , {1, 0} , },
};

int8_t hardwareExpsMapArray[NUM_PRESETS][NUM_HARDWARE_EXPS][2] = {
    {  {-1, -1} , {-1, -1} , },
};

const int8_t PROGMEM midiAssignsArray[NUM_PRESETS][MAX_NUM_MIDI_ASSIGNS][4] = {
    {  },
};

const uint16_t PROGMEM midiRoutesArray[NUM_PRESETS][MAX_NUM_MIDI_ROUTES][2] = {
    {  },
};

const char* const PROGMEM presetNamesArray[] = {
    "New Preset","000",
};

const int8_t PROGMEM presetTraversalArray[] = {
-1,2,-1,5,2,0,-1,0,3,1,4,1,-1,1,0,3,1,3,-1,3,-1,4,};

const uint16_t PROGMEM presetTraversalStartSizeArray[] = {
0,22,};

#include "Tremolo.h"
#include "PlateReverb.h"


using namespace Avalon;

#include "Avalon/PresetControlArrays.h"
PresetControlArrays presetControlArrays(
    (int8_t*)   connectionsArray,
    (int8_t*)   streamConvertArray,
    (uint32_t*) PRESET_CHECKSUMS,
    (float*)    flashPresetControlValues,
    (const char* const*)  globalInstanceNamesArray,
    (const char* const*)  effectControlNamesArray,
    (const uint8_t*)      effectControlEnumIndexArray,
    (const char* const*)  effectControlEnumNamesArray,
    (const float*)        effectControlSpecsArray,
    (const uint16_t*)     effectControlUserArray,
    (const char* const*)  IrLoader::IMPULSE_RESPONSE_NAME_PTRS,
    (int16_t*)effectControlStartLookup,
    (int8_t*) effectControlSizeLookup,
    (int8_t*) hardwareEncodersMapArray,
    (int8_t*) hardwareSwitchesMapArray,
    (int8_t*) hardwareLedsMapArray,
    (int8_t*) hardwareExpsMapArray,
    (const int8_t*)      midiAssignsArray,
    (const uint16_t*)    midiRoutesArray,
    (const char* const*) presetNamesArray,
    (const int8_t*)      presetTraversalArray,
    (const uint16_t*)    presetTraversalStartSizeArray,
    (unsigned) NUM_PRESETS,
    (unsigned) NUM_PANEL_OUTPUTS,
    (unsigned) MAX_EFFECTS_PER_PRESET,
    (unsigned) MAX_CONTROLS_PER_EFFECT,
    (unsigned) NUM_GLOBAL_INSTANCES,
    (unsigned) NUM_AUDIO_IO_INSTANCES,
    (unsigned) MAX_NUM_MIDI_ASSIGNS,
    (unsigned) MAX_NUM_MIDI_ROUTES,
    (unsigned) EFFECT_CONTROL_ENUM_INDEX_ARRAY_SIZE
);
