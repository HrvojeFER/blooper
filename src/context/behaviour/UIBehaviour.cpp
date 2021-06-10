#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}

UIBehaviour::~UIBehaviour() = default;


// Selection and Focus

te::Edit* UIBehaviour::getCurrentlyFocusedEdit()
{
  if (auto selection = this->getCurrentlyFocusedSelectionManager())
  {
    if (auto track = selection->getFirstItemOfType<EditTrack>())
    {
      return std::addressof(track->getEdit());
    }
  }

  return nullptr;
}

te::Edit* UIBehaviour::getLastFocusedEdit()
{
  if (auto selection = this->getCurrentlyFocusedSelectionManager())
  {
    if (auto track = selection->getFirstItemOfType<EditTrack>())
    {
      return std::addressof(track->getEdit());
    }
  }

  return nullptr;
}

juce::Array<te::Edit*> UIBehaviour::getAllOpenEdits()
{
  if (auto fullContext = dynamic_cast<AbstractContext*>(
          std::addressof(this->getContext())))
  {
    juce::Array<te::Edit*> result;

    fullContext->getEditManager().visit(
        [&result](EditTrack* track) {
          result.add(std::addressof(track->getEdit()));
        });

    return result;
  }

  return {};
}

bool UIBehaviour::isEditVisibleOnScreen(const te::Edit&)
{
  // TODO
  if (dynamic_cast<AbstractContext*>(std::addressof(this->getContext())))
    return true;

  return false;
}

bool UIBehaviour::closeAllEditsBelongingToProject(te::Project& project)
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
  {
    if (fullContext->getProject().getProjectFile() == project.getProjectFile())
      BLOOPER_ASSERT(false);
  }

  return true;
}


// TODO?

te::SelectionManager* UIBehaviour::getCurrentlyFocusedSelectionManager()
{
  auto& manager = this->getContext().getSelectionManager();

  return std::addressof(manager);
}

te::SelectionManager* UIBehaviour::getSelectionManagerForRack(
    const te::RackType&)
{
  auto& manager = this->getContext().getSelectionManager();

  return std::addressof(manager);
}


te::Project::Ptr UIBehaviour::getCurrentlyFocusedProject()
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
    return std::addressof(fullContext->getProject());

  return nullptr;
}


// TODO?

void UIBehaviour::selectProjectInFocusedWindow(te::Project::Ptr ptr)
{
  te::UIBehaviour::selectProjectInFocusedWindow(ptr);
}


// Commands

juce::ApplicationCommandManager* UIBehaviour::getApplicationCommandManager()
{
  auto& manager = this->getContext().getCommandManager();

  return std::addressof(manager);
}

void UIBehaviour::getAllCommands(juce::Array<juce::CommandID>& array)
{
  auto& manager = this->getContext().getCommandManager();

  for (const auto& category : manager.getCommandCategories())
    array.addArray(manager.getCommandsInCategory(category));
}

void UIBehaviour::getCommandInfo(
    juce::CommandID               commandId,
    juce::ApplicationCommandInfo& info)
{
  auto& manager = this->getContext().getCommandManager();

  info = *manager.getCommandForID(commandId);
}

bool UIBehaviour::perform(
    const juce::ApplicationCommandTarget::InvocationInfo& info)
{
  auto& manager = this->getContext().getCommandManager();

  return manager.invoke(info, true);
}


bool UIBehaviour::paste(const te::Clipboard& clipboard)
{
  // TODO?
  return te::UIBehaviour::paste(clipboard);
}


// TODO?

void UIBehaviour::toggleScroll()
{
  te::UIBehaviour::toggleScroll();
}

void UIBehaviour::scrollTracksUp()
{
  te::UIBehaviour::scrollTracksUp();
}

void UIBehaviour::scrollTracksDown()
{
  te::UIBehaviour::scrollTracksDown();
}

void UIBehaviour::scrollTracksLeft()
{
  te::UIBehaviour::scrollTracksLeft();
}

void UIBehaviour::scrollTracksRight()
{
  te::UIBehaviour::scrollTracksRight();
}


// TODO?

void UIBehaviour::nudgeSelectedClips(
    te::TimecodeSnapType          snap_type,
    const juce::String&           commandDesc,
    te::SelectionManager&         manager,
    const juce::Array<te::Clip*>& array,
    bool                          automationLocked)
{
  te::UIBehaviour::nudgeSelectedClips(
      snap_type,
      commandDesc,
      manager,
      array,
      automationLocked);
}

void UIBehaviour::nudgeSelected(
    te::TimecodeSnapType snap_type,
    const juce::String&  commandDesc,
    bool                 automationLocked)
{
  te::UIBehaviour::nudgeSelected(
      snap_type,
      commandDesc,
      automationLocked);
}

void UIBehaviour::nudgeSelected(
    const juce::String& commandDesc)
{
  te::UIBehaviour::nudgeSelected(
      commandDesc);
}


// TODO?

void UIBehaviour::stopPreviewPlayback()
{
  te::UIBehaviour::stopPreviewPlayback();
}


// TODO?

void UIBehaviour::resetOverloads()
{
  te::UIBehaviour::resetOverloads();
}


// TODO?

void UIBehaviour::zoomHorizontal(float d)
{
  te::UIBehaviour::zoomHorizontal(d);
}

void UIBehaviour::zoomVertical(float d)
{
  te::UIBehaviour::zoomVertical(d);
}

void UIBehaviour::zoomToSelection()
{
  te::UIBehaviour::zoomToSelection();
}

void UIBehaviour::zoomToFitHorizontally()
{
  te::UIBehaviour::zoomToFitHorizontally();
}

void UIBehaviour::zoomToFitVertically()
{
  te::UIBehaviour::zoomToFitVertically();
}


// TODO?

void UIBehaviour::updateAllProjectItemLists()
{
  te::UIBehaviour::updateAllProjectItemLists();
}


// Windows

// TODO?

void UIBehaviour::showWarningAlert(
    const juce::String& title,
    const juce::String& message)
{
  te::UIBehaviour::showWarningAlert(
      title,
      message);
}

bool UIBehaviour::showOkCancelAlertBox(
    const juce::String& title,
    const juce::String& message,
    const juce::String& ok,
    const juce::String& cancel)
{
  return te::UIBehaviour::showOkCancelAlertBox(
      title,
      message,
      ok,
      cancel);
}

int UIBehaviour::showYesNoCancelAlertBox(
    const juce::String& title,
    const juce::String& message,
    const juce::String& yes,
    const juce::String& no,
    const juce::String& cancel)
{
  return te::UIBehaviour::showYesNoCancelAlertBox(
      title,
      message,
      yes,
      no,
      cancel);
}

void UIBehaviour::showInfoMessage(
    const juce::String& message)
{
  te::UIBehaviour::showInfoMessage(
      message);
}

void UIBehaviour::showWarningMessage(
    const juce::String& message)
{
  te::UIBehaviour::showWarningMessage(
      message);
}


// TODO?

void UIBehaviour::showQuantisationLevel()
{
  te::UIBehaviour::showQuantisationLevel();
}


// TODO?

void UIBehaviour::runTaskWithProgressBar(
    te::ThreadPoolJobWithProgress& progress)
{
  te::UIBehaviour::runTaskWithProgressBar(progress);
}


// TODO?

void UIBehaviour::showSafeRecordDialog(te::TransportControl& control)
{
  te::UIBehaviour::showSafeRecordDialog(control);
}

void UIBehaviour::hideSafeRecordDialog(te::TransportControl& control)
{
  te::UIBehaviour::hideSafeRecordDialog(control);
}


te::Plugin::Ptr UIBehaviour::showMenuAndCreatePlugin(
    te::Plugin::Type type,
    te::Edit&        edit)
{
  // TODO: with plugin picker
  return te::UIBehaviour::showMenuAndCreatePlugin(type, edit);
}

std::unique_ptr<JuceComponent> UIBehaviour::createPluginWindow(
    JucePluginWindowState& windowState)
{
  if (auto concreteWindowState = dynamic_cast<te::Plugin::WindowState*>(
          &windowState))
  {
    PluginEditorWindow::Options windowOptions{};

    return std::unique_ptr<JuceComponent>{
        showPluginEditorWindow(
            getContext(),
            concreteWindowState->plugin,
            move(windowOptions))};
  }

  return te::UIBehaviour::createPluginWindow(windowState);
}

void UIBehaviour::recreatePluginWindowContentAsync(
    JucePlugin& plugin)
{
  if (auto window = dynamic_cast<PluginEditorWindow*>(
          plugin.windowState->pluginWindow.get()))
    return util::callAsync(
        [safeWindow =
             juce::Component::SafePointer<PluginEditorWindow>(
                 window)]() mutable {
          if (safeWindow) safeWindow->recreateContent();
        });

  return te::UIBehaviour::recreatePluginWindowContentAsync(plugin);
}


// TODO: rest?

void UIBehaviour::showProjectScreen()
{
  showProjectsMenu(getContext());
}

void UIBehaviour::showSettingsScreen()
{
  showSettingsMenu(getContext());
}

void UIBehaviour::showEditScreen()
{
  te::UIBehaviour::showEditScreen();
}

void UIBehaviour::showHideVideo()
{
  te::UIBehaviour::showHideVideo();
}

void UIBehaviour::showHideInputs()
{
  te::UIBehaviour::showHideInputs();
}

void UIBehaviour::showHideOutputs()
{
  te::UIBehaviour::showHideOutputs();
}

void UIBehaviour::showHideAllPanes()
{
  te::UIBehaviour::showHideAllPanes();
}


// Properties

// TODO?

double UIBehaviour::getEditingPosition(te::Edit& edit)
{
  return te::UIBehaviour::getEditingPosition(edit);
}

te::EditTimeRange UIBehaviour::getEditingRange(te::Edit& edit)
{
  return te::UIBehaviour::getEditingRange(edit);
}

juce::Array<te::Track*> UIBehaviour::getEditingTracks(te::Edit& edit)
{
  return te::UIBehaviour::getEditingTracks(edit);
}

te::SelectableList UIBehaviour::getAssociatedClipsToEdit(
    const te::SelectableList& items)
{
  return te::UIBehaviour::getAssociatedClipsToEdit(items);
}


// TODO?

bool UIBehaviour::shouldGenerateLiveWaveformsWhenRecording()
{
  return te::UIBehaviour::shouldGenerateLiveWaveformsWhenRecording();
}


// TODO?

bool UIBehaviour::getBigInputMetersMode()
{
  return te::UIBehaviour::getBigInputMetersMode();
}

void UIBehaviour::setBigInputMetersMode(bool b)
{
  te::UIBehaviour::setBigInputMetersMode(b);
}


// Hooks

// TODO?

void UIBehaviour::newTrackCreated(te::Track& track)
{
  te::UIBehaviour::newTrackCreated(track);
}


// TODO?

void UIBehaviour::editNamesMayHaveChanged()
{
  te::UIBehaviour::editNamesMayHaveChanged();
}

BLOOPER_NAMESPACE_END
