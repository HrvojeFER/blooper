#ifndef BLOOPER_ENGINE_BEHAVIOUR_HPP
#define BLOOPER_ENGINE_BEHAVIOUR_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EngineBehaviour :
    public CoreContextualBase,
    public te::EngineBehaviour
{
 public:
  explicit EngineBehaviour(AbstractCoreContext& context);
  ~EngineBehaviour() override;


  // Devices

  bool autoInitialiseDeviceManager() override;

  bool isDescriptionOfWaveDevicesSupported() override;

  void describeWaveDevices(
      std::vector<te::WaveDeviceDescription>& vector,
      juce::AudioIODevice&                    device,
      bool                                    b) override;


  // Plugins

  juce::ReferenceCountedObjectPtr<te::RackType> createPresetRackType(
      int       i,
      te::Edit& edit) override;

  te::Plugin::Ptr createCustomPlugin(
      te::PluginCreationInfo info) override;


  std::unique_ptr<juce::PluginDescription> findDescriptionForFileOrID(
      const juce::String& string) override;

  bool isPluginDisabled(const juce::String& string) override;

  void setPluginDisabled(const juce::String& string, bool b) override;

  void saveCustomPluginProperties(
      juce::ValueTree&           tree,
      juce::AudioPluginInstance& instance,
      juce::UndoManager*         manager) override;


  bool canScanPluginsOutOfProcess() override;


  // Playback

  int getMiddleCOctave() override;

  void setMiddleCOctave(int i) override;


  bool lengthOfOneBeatDependsOnTimeSignature() override;


  LevelMeterSettings getLevelMeterSettings() override;

  void setLevelMeterSettings(LevelMeterSettings settings) override;


  // Remapping

  bool areAudioClipsRemappedWhenTempoChanges() override;

  void setAudioClipsRemappedWhenTempoChanges(bool b) override;


  bool areAutoTempoClipsRemappedWhenTempoChanges() override;

  void setAutoTempoClipsRemappedWhenTempoChanges(bool b) override;


  bool areMidiClipsRemappedWhenTempoChanges() override;

  void setMidiClipsRemappedWhenTempoChanges(bool b) override;


  bool arePluginsRemappedWhenTempoChanges() override;

  void setPluginsRemappedWhenTempoChanges(bool b) override;


  // Properties

  bool isMidiDriverUsedForIncommingMessageTiming() override;

  void setMidiDriverUsedForIncommingMessageTiming(bool b) override;

  bool shouldPlayMidiGuideNotes() override;


  int getNumberOfCPUsToUseForAudio() override;

  void setProcessPriority(int i) override;


  int getDefaultLoopedSequenceType() override;


  // Hooks

  void editHasBeenSaved(te::Edit& edit, juce::File file) override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineBehaviour)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ENGINE_BEHAVIOUR_HPP
