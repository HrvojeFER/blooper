#include <blooper/context/behaviour/EngineBehaviour.hpp>

BLOOPER_NAMESPACE_BEGIN

EngineBehaviour::EngineBehaviour(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}

EngineBehaviour::~EngineBehaviour() = default;


// Devices

bool EngineBehaviour::autoInitialiseDeviceManager()
{
  return te::EngineBehaviour::autoInitialiseDeviceManager();
}


bool EngineBehaviour::isDescriptionOfWaveDevicesSupported()
{
  return te::EngineBehaviour::isDescriptionOfWaveDevicesSupported();
}

void EngineBehaviour::describeWaveDevices(
    std::vector<te::WaveDeviceDescription>& vector,
    juce::AudioIODevice&                    device,
    bool                                    b)
{
  te::EngineBehaviour::describeWaveDevices(
      vector,
      device,
      b);
}


// Plugins

juce::ReferenceCountedObjectPtr<te::RackType>
EngineBehaviour::createPresetRackType(int i, te::Edit& edit)
{
  return te::EngineBehaviour::createPresetRackType(i, edit);
}

te::Plugin::Ptr
EngineBehaviour::createCustomPlugin(te::PluginCreationInfo info)
{
  return te::EngineBehaviour::createCustomPlugin(info);
}


std::unique_ptr<juce::PluginDescription>
EngineBehaviour::findDescriptionForFileOrID(const juce::String& string)
{
  return te::EngineBehaviour::findDescriptionForFileOrID(string);
}

bool EngineBehaviour::isPluginDisabled(const juce::String& string)
{
  return te::EngineBehaviour::isPluginDisabled(string);
}

void EngineBehaviour::setPluginDisabled(const juce::String& string, bool b)
{
  te::EngineBehaviour::setPluginDisabled(string, b);
}

void EngineBehaviour::saveCustomPluginProperties(
    juce::ValueTree&           tree,
    juce::AudioPluginInstance& instance,
    juce::UndoManager*         manager)
{
  te::EngineBehaviour::saveCustomPluginProperties(
      tree,
      instance,
      manager);
}


bool EngineBehaviour::canScanPluginsOutOfProcess()
{
  return te::EngineBehaviour::canScanPluginsOutOfProcess();
}


// Playback

int EngineBehaviour::getMiddleCOctave()
{
  return te::EngineBehaviour::getMiddleCOctave();
}

void EngineBehaviour::setMiddleCOctave(int i)
{
  te::EngineBehaviour::setMiddleCOctave(i);
}


bool EngineBehaviour::lengthOfOneBeatDependsOnTimeSignature()
{
  return te::EngineBehaviour::lengthOfOneBeatDependsOnTimeSignature();
}


EngineBehaviour::LevelMeterSettings EngineBehaviour::getLevelMeterSettings()
{
  return te::EngineBehaviour::getLevelMeterSettings();
}

void EngineBehaviour::setLevelMeterSettings(
    EngineBehaviour::LevelMeterSettings settings)
{
  te::EngineBehaviour::setLevelMeterSettings(settings);
}


// Remapping

bool EngineBehaviour::areAudioClipsRemappedWhenTempoChanges()
{
  return te::EngineBehaviour::areAudioClipsRemappedWhenTempoChanges();
}

void EngineBehaviour::setAudioClipsRemappedWhenTempoChanges(bool b)
{
  te::EngineBehaviour::setAudioClipsRemappedWhenTempoChanges(b);
}


bool EngineBehaviour::areAutoTempoClipsRemappedWhenTempoChanges()
{
  return te::EngineBehaviour::areAutoTempoClipsRemappedWhenTempoChanges();
}

void EngineBehaviour::setAutoTempoClipsRemappedWhenTempoChanges(bool b)
{
  te::EngineBehaviour::setAutoTempoClipsRemappedWhenTempoChanges(b);
}


bool EngineBehaviour::areMidiClipsRemappedWhenTempoChanges()
{
  return te::EngineBehaviour::areMidiClipsRemappedWhenTempoChanges();
}

void EngineBehaviour::setMidiClipsRemappedWhenTempoChanges(bool b)
{
  te::EngineBehaviour::setMidiClipsRemappedWhenTempoChanges(b);
}


bool EngineBehaviour::arePluginsRemappedWhenTempoChanges()
{
  return te::EngineBehaviour::arePluginsRemappedWhenTempoChanges();
}

void EngineBehaviour::setPluginsRemappedWhenTempoChanges(bool b)
{
  te::EngineBehaviour::setPluginsRemappedWhenTempoChanges(b);
}


// Properties

bool EngineBehaviour::isMidiDriverUsedForIncommingMessageTiming()
{
  return te::EngineBehaviour::isMidiDriverUsedForIncommingMessageTiming();
}

void EngineBehaviour::setMidiDriverUsedForIncommingMessageTiming(bool b)
{
  te::EngineBehaviour::setMidiDriverUsedForIncommingMessageTiming(b);
}

bool EngineBehaviour::shouldPlayMidiGuideNotes()
{
  return te::EngineBehaviour::shouldPlayMidiGuideNotes();
}


int EngineBehaviour::getNumberOfCPUsToUseForAudio()
{
  return te::EngineBehaviour::getNumberOfCPUsToUseForAudio();
}

void EngineBehaviour::setProcessPriority(int i)
{
  te::EngineBehaviour::setProcessPriority(i);
}


int EngineBehaviour::getDefaultLoopedSequenceType()
{
  return te::EngineBehaviour::getDefaultLoopedSequenceType();
}


// Hooks

void EngineBehaviour::editHasBeenSaved(te::Edit& edit, juce::File file)
{
  te::EngineBehaviour::editHasBeenSaved(edit, file);
}

BLOOPER_NAMESPACE_END
