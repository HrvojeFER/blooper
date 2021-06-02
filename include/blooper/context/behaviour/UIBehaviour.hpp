#ifndef BLOOPER_UI_BEHAVIOUR_HPP
#define BLOOPER_UI_BEHAVIOUR_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class UIBehaviour :
    public CoreContextualBase,
    public te::UIBehaviour
{
 public:
  explicit UIBehaviour(AbstractCoreContext& context);

  ~UIBehaviour() override;


  // Selection and Focus

  te::Edit* getCurrentlyFocusedEdit() override;

  te::Edit* getLastFocusedEdit() override;

  juce::Array<te::Edit*> getAllOpenEdits() override;

  bool isEditVisibleOnScreen(const te::Edit& edit) override;


  te::SelectionManager* getCurrentlyFocusedSelectionManager() override;

  te::SelectionManager* getSelectionManagerForRack(
      const te::RackType& rack_type) override;


  te::Project::Ptr getCurrentlyFocusedProject() override;

  void selectProjectInFocusedWindow(te::Project::Ptr ptr) override;


  // Commands

  juce::ApplicationCommandManager* getApplicationCommandManager() override;

  void getAllCommands(
      juce::Array<juce::CommandID>& array) override;
  void getCommandInfo(
      juce::CommandID               commandId,
      juce::ApplicationCommandInfo& info) override;

  bool perform(
      const juce::ApplicationCommandTarget::InvocationInfo& info) override;


  bool paste(const te::Clipboard& clipboard) override;


  void toggleScroll() override;

  void scrollTracksUp() override;

  void scrollTracksDown() override;

  void scrollTracksLeft() override;

  void scrollTracksRight() override;


  void nudgeSelectedClips(
      te::TimecodeSnapType          snap_type,
      const juce::String&           commandDesc,
      te::SelectionManager&         manager,
      const juce::Array<te::Clip*>& array,
      bool                          automationLocked) override;

  void nudgeSelected(
      te::TimecodeSnapType snap_type,
      const juce::String&  commandDesc,
      bool                 automationLocked) override;

  void nudgeSelected(
      const juce::String& commandDesc) override;


  void stopPreviewPlayback() override;


  void resetOverloads() override;


  void zoomHorizontal(float d) override;

  void zoomVertical(float d) override;

  void zoomToSelection() override;

  void zoomToFitHorizontally() override;

  void zoomToFitVertically() override;


  bool closeAllEditsBelongingToProject(te::Project& project) override;


  void updateAllProjectItemLists() override;


  // Windows

  void showWarningAlert(
      const juce::String& title,
      const juce::String& message) override;

  bool showOkCancelAlertBox(
      const juce::String& title,
      const juce::String& message,
      const juce::String& ok,
      const juce::String& cancel) override;

  int showYesNoCancelAlertBox(
      const juce::String& title,
      const juce::String& message,
      const juce::String& yes,
      const juce::String& no,
      const juce::String& cancel) override;

  void showInfoMessage(
      const juce::String& message) override;

  void showWarningMessage(
      const juce::String& message) override;


  void showQuantisationLevel() override;


  void runTaskWithProgressBar(
      te::ThreadPoolJobWithProgress& progress) override;


  void showSafeRecordDialog(te::TransportControl& control) override;

  void hideSafeRecordDialog(te::TransportControl& control) override;


  std::unique_ptr<JuceComponent> createPluginWindow(
      JucePluginWindowState& windowState) override;

  void recreatePluginWindowContentAsync(
      JucePlugin& plugin) override;

  te::Plugin::Ptr showMenuAndCreatePlugin(
      te::Plugin::Type type,
      te::Edit&        edit) override;


  void showProjectScreen() override;

  void showSettingsScreen() override;

  void showEditScreen() override;

  void showHideVideo() override;

  void showHideInputs() override;

  void showHideOutputs() override;

  void showHideAllPanes() override;


  // Properties

  double getEditingPosition(te::Edit& edit) override;

  te::EditTimeRange getEditingRange(te::Edit& edit) override;

  juce::Array<te::Track*> getEditingTracks(te::Edit& edit) override;

  te::SelectableList getAssociatedClipsToEdit(
      const te::SelectableList& items) override;


  bool shouldGenerateLiveWaveformsWhenRecording() override;


  bool getBigInputMetersMode() override;

  void setBigInputMetersMode(bool b) override;


  // Hooks

  void newTrackCreated(te::Track& track) override;

  void editNamesMayHaveChanged() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIBehaviour);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_UI_BEHAVIOUR_HPP
