#include <blooper/context/behaviour/UIBehaviour.hpp>

#include <blooper/internal/utils/style.hpp>

#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/context/plugins/PluginEditorWindow.hpp>
#include <blooper/context/plugins/PluginPickerComponent.hpp>
#include <blooper/context/settings/SettingsMenuWindow.hpp>
#include <blooper/context/projects/ProjectsMenuWindow.hpp>

BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}

UIBehaviour::~UIBehaviour() = default;


// Selection and Focus

te::Edit* UIBehaviour::getCurrentlyFocusedEdit()
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
  {
    return fullContext->getFocusedEdit().get();
  }

  return nullptr;
}

te::Edit* UIBehaviour::getLastFocusedEdit()
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
  {
    return fullContext->getFocusedEdit().get();
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
        [&result](const JuceEditRef& edit) {
          result.add(edit.get());
        });

    return result;
  }

  return {};
}

bool UIBehaviour::isEditVisibleOnScreen(const te::Edit& edit)
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
  {
    return fullContext->getFocusedEdit()->getProjectItemID() ==
           edit.getProjectItemID();
  }

  return false;
}

// TODO?

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


te::SelectionManager* UIBehaviour::getCurrentlyFocusedSelectionManager()
{
  return this->getContext().getFocusedSelectionManager();
}

te::SelectionManager* UIBehaviour::getSelectionManagerForRack(
    const te::RackType&)
{
  return this->getContext().getFocusedSelectionManager();
}


te::Project::Ptr UIBehaviour::getCurrentlyFocusedProject()
{
  if (auto fullContext =
          dynamic_cast<AbstractContext*>(
              std::addressof(this->getContext())))
  {
    return std::addressof(fullContext->getProject());
  }

  return nullptr;
}


void UIBehaviour::selectProjectInFocusedWindow(te::Project::Ptr project)
{
  this->getContext().openProject(move(project));
}


// Commands

juce::ApplicationCommandManager* UIBehaviour::getApplicationCommandManager()
{
  auto& manager = this->getContext().getCommandManager();

  return std::addressof(manager);
}

void UIBehaviour::getAllCommands(juce::Array<juce::CommandID>& array)
{
  if (auto commandContext =
          dynamic_cast<juce::ApplicationCommandTarget*>(
              std::addressof(this->getContext())))
  {
    commandContext->getAllCommands(array);
  }
}

void UIBehaviour::getCommandInfo(
    juce::CommandID               commandId,
    juce::ApplicationCommandInfo& info)
{
  if (auto commandContext =
          dynamic_cast<juce::ApplicationCommandTarget*>(
              std::addressof(this->getContext())))
  {
    commandContext->getCommandInfo(commandId, info);
  }
}

bool UIBehaviour::perform(
    const juce::ApplicationCommandTarget::InvocationInfo& info)
{
  if (auto commandContext =
          dynamic_cast<juce::ApplicationCommandTarget*>(
              std::addressof(this->getContext())))
  {
    return commandContext->perform(info);
  }

  return false;
}


bool UIBehaviour::paste(const te::Clipboard&)
{
  return this->getContext().getSelectionManager().pasteSelected();
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

// in tracktion_engine messages are painted black because
// the engine doesn't set the attributed message colour

void UIBehaviour::showInfoMessage(
    const juce::String& message)
{
  if (auto mouseComponent =
          juce::Desktop::getInstance()
              .getMainMouseSource()
              .getComponentUnderMouse())
  {
    juce::AttributedString attributedMessage{message};
    attributedMessage.setColour(
        this->getContext()
            .getLookAndFeel()
            .findColour(ColourId::white));

    auto bubble = new juce::BubbleMessageComponent();
    bubble->addToDesktop(0);
    bubble->showAt(
        mouseComponent,
        move(attributedMessage),
        2000,
        true,
        true);
  }
}

void UIBehaviour::showWarningMessage(
    const juce::String& message)
{
  if (auto mouseComponent =
          juce::Desktop::getInstance()
              .getMainMouseSource()
              .getComponentUnderMouse())
  {
    juce::AttributedString attributedMessage{message};
    attributedMessage.setColour(
        this->getContext()
            .getLookAndFeel()
            .findColour(ColourId::red));

    auto bubble = new juce::BubbleMessageComponent();
    bubble->addToDesktop(0);
    bubble->showAt(
        mouseComponent,
        move(attributedMessage),
        2000,
        true,
        true);
  }
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
  if (auto fullContext = dynamic_cast<AbstractContext*>(
          std::addressof(this->getContext())))
  {
    PluginPickerComponent::Options pickerOptions{};

    return pickPlugin(*fullContext, move(pickerOptions));
  }

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
  {
    return static_cast<void>(
        juce::MessageManager::callAsync(
            [window =
                 juce::Component::SafePointer<PluginEditorWindow>(
                     window)]() mutable {
              if (!window) return;
              window->recreateContent();
            }));
  }

  return te::UIBehaviour::recreatePluginWindowContentAsync(plugin);
}


// TODO: rest?

void UIBehaviour::showProjectScreen()
{
  showProjectsMenu(this->getContext());
}

void UIBehaviour::showSettingsScreen()
{
  showSettingsMenu(this->getContext());
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
