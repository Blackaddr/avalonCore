#include <cmath>
#include "sysPlatform/SysTypes.h"
#include "sysPlatform/SysDebugPrint.h"
#include "sysPlatform/SysCpuControl.h"
#include "sysPlatform/SysTimer.h"
#include "sysPlatform/SysIO.h"
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

#include "presets.h"

using namespace SysPlatform;
using namespace Avalon;
using namespace Stride;

/// START PASTE ////////////////////////////////////////////////////////
BlackaddrAudio_Tremolo::Tremolo _BlackaddrAudio_Tremolo0;
HexeFX_PlateReverb::PlateReverb _HexeFX_PlateReverb0;
Avalon::AudioEffectOutputLevel outputLevel0;
SysAudioInputI2S i2sIn0;

uint8_t watermark[32] = { 0x65, 0xd8, 0x12, 0xae, 0x1d, 0xf2, 0x85, 0x98, 0x4d, 0xf3, 0xea, 0x0d, 0x22, 0x7d, 0x3e, 0x33, 0x20, 0x5c, 0xf4, 0x82, 0x65, 0xfd, 0x48, 0xf0, 0x87, 0x97, 0xe8, 0xe6, 0x9e, 0xb8, 0xfe, 0x85};
char BUILD_TIME[BUILD_TIME_SIZE+1] = __TIME__; // +1 for null terminator
SramManager sramManager(true);

ElapsedMillis timer;

AudioConnectionPanel connectionPanel(NUM_PANEL_INPUTS,NUM_PANEL_OUTPUTS);
AudioEffectMeasure inputMeasureX;
SysAudioOutputI2S i2sOut0;
AudioEffectWrapper* audioEffectPtrArray[4] = {(AudioEffectWrapper*)&_BlackaddrAudio_Tremolo0,(AudioEffectWrapper*)&_HexeFX_PlateReverb0,nullptr,nullptr,};
SysAudioIO* sysAudioIoPtrArray[2] = {(SysAudioIO*)&i2sIn0,(SysAudioIO*)&i2sOut0,};

FlashManager flashManager((float*)presetControlValues, (float*)flashPresetControlValues, NUM_PRESETS, MAX_EFFECTS_PER_PRESET, MAX_CONTROLS_PER_EFFECT);
PresetManager presetManager(flashManager, sramManager, &connectionPanel, presetControlArrays, audioEffectPtrArray, sysAudioIoPtrArray, (float*)presetControlValues);
AvalonHardwareControls hardwareControls(presetManager, outputLevel0, &inputMeasureX, (int8_t*)connectionsArray, (int16_t*)effectControlStartLookup, (int8_t*)effectControlSizeLookup,     (char**) effectControlNamesArray, (float*)effectControlSpecsArray,
    (uint16_t*)effectControlUserArray,(char**)globalInstanceNamesArray, (char**)IrLoader::IMPULSE_RESPONSE_NAME_PTRS, (int8_t*)hardwareEncodersMapArray, (int8_t*)hardwareSwitchesMapArray, (int8_t*)hardwareLedsMapArray, (int8_t*)hardwareExpsMapArray,
    NUM_GLOBAL_INSTANCES, NUM_PRESETS, IrLoader::NUM_IMPULSE_RESPONSES, presetControlArrays);
MidiManager midiManager(presetManager, hardwareControls, presetControlArrays);
ReportManager reportManager(presetManager, hardwareControls, presetControlArrays, audioEffectPtrArray);
TunerManager tunerManager(hardwareControls);

AudioConnection outL(outputLevel0, 0, i2sOut0, 0);
AudioConnection outR(outputLevel0, 1, i2sOut0, 1);
AudioConnection pin2(_BlackaddrAudio_Tremolo0, 0, connectionPanel, 2);
AudioConnection pin3(_HexeFX_PlateReverb0, 0, connectionPanel, 3);
AudioConnection pin4(_HexeFX_PlateReverb0, 1, connectionPanel, 4);
AudioConnection pin0(i2sIn0, 0, connectionPanel, 0);
AudioConnection pin1(i2sIn0, 1, connectionPanel, 1);

AudioConnection pout2(connectionPanel, 2, _BlackaddrAudio_Tremolo0, 0);
AudioConnection pout3(connectionPanel, 3, _HexeFX_PlateReverb0, 0);
AudioConnection pout4(connectionPanel, 4, _HexeFX_PlateReverb0, 1);
AudioConnection pout0(connectionPanel, 0, outputLevel0, 0);
AudioConnection pout1(connectionPanel, 1, outputLevel0, 1);

AudioConnection pmeasure(i2sIn0, 0, inputMeasureX, 0);
////////////////////////////////////////////////////////////////

// AudioConnection outL(i2sIn0, 0, i2sOut0, 0);
// AudioConnection outR(i2sIn0, 1, i2sOut0, 1);

void setup() {
    sysEnableDebugPrint();
    sysInitialize();
    sysInitShowSummary();
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.begin(57600));
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("setup(): entered\n"); SysPlatform::sysLogger.flush(););
    if (!flashManager.isFirstBoot() && sysCrashReport) { midiManager.createCrashReport(); sysCrashReport.printReport(); }
    else {sysCrashReport.reset(); }
    sysCrashReport.setBreadcrumb(SysCrashReport::SETUP_ID, SysCrashReport::START_MASK, 0);
    sramManagerPtr = &sramManager;
    sramManager.begin();
    reportManagerPtr = &reportManager;
    tunerManagerPtr  = &tunerManager;
    memcpy((void*)presetControlValues, (void*)flashPresetControlValues, NUM_PRESETS*MAX_EFFECTS_PER_PRESET*MAX_CONTROLS_PER_EFFECT*sizeof(float));
    SysCpuTelemetry::markUnusedStack();
    _BlackaddrAudio_Tremolo0.setId(0);
    _HexeFX_PlateReverb0.setId(1);
    i2sIn0.setId(2); i2sIn0.begin();
    outputLevel0.setId(3);
    connectionPanel.setId(AudioStream::UPDATE_STEP_OBJECT_ID);
    i2sOut0.setId(4); i2sOut0.begin();
    inputMeasureX.setId(5);
    AudioStream::setOrderedUpdate(true);
    hardwareControls.setMidiManagerPtr(&midiManager);
    AudioMemory(17);
    hardwareControls.setMidiManagerPtr(&midiManager);
    SysTimer::delayMilliseconds(100); 

    showMemoryConfig();
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("Initial stack used: %u bytes, %f percent\n", getStackUsageBytes(), getStackUsageRatio()*100.0f);)
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("Initial heap used: %u bytes, %f percent\n", getHeapUsageBytes(), getHeapUsageRatio()*100.0f);)
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("Build Time is %s\n", BUILD_TIME));

    if (!flashManager.init()) { SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("setup(): FlashManager returned error during init")); }
    hardwareControls.startControlsThread(&hardwareControls);
    SysCpuControl::yield();

    connectionPanel.enable();
    inputMeasureX.enable();
    inputMeasureX.bypass(false);
    inputMeasureX.setPeakEnable(true);
    sysCrashReport.setBreadcrumb(SysCrashReport::SETUP_ID, SysCrashReport::DONE_MASK, 0);
    SYS_DEBUG_PRINT(SysPlatform::sysLogger.printf("setup(): DONE!\n"); SysPlatform::sysLogger.flush(););
}

void loop() {
    static bool controlLoopStarted = false;
    static unsigned timerCount = 0;
    if (timer > 4000) {
        SYS_DEBUG_PRINT(sysLogger.printf("Loop %d, temp: %0.1f C\n\r", timerCount++, SysCpuTelemetry::getTemperatureCelcius()));
        timer = 0;
    }
    if (!controlLoopStarted) { controlLoopStarted = hardwareControls.isControlLoopRunning(); }
    else { 
        midiManager.processMidiInput();
        sysIO.scanInputs();
    }
    SysPlatform::SysCpuControl::yield();
}
