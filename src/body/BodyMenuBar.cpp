#include <blooper/body/BodyMenuBar.hpp>

#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] BodyMenuBar::BodyMenuBar(
    AbstractContext& context,
    State            state,
    Options          options)
    : ContextualBase(context),
      StatefulBase(move(state)),
      options(move(options))
{
  this->setApplicationCommandManagerToWatch(
      std::addressof(this->getContext().getCommandManager()));
}


[[maybe_unused]] juce::StringArray BodyMenuBar::getMenuBarNames()
{
  return {"File", "Edit", "View", "Help"};
}

[[maybe_unused]] juce::PopupMenu BodyMenuBar::getMenuForIndex(
    int index,
    const juce::String&)
{
  juce::PopupMenu result{};

  auto commandManager =
      std::addressof(this->getContext().getCommandManager());

  auto& assetManager =
      this->getContext().getAssetManager();

  switch (index)
  {
    case 0: // File
      result.addCommandItem(
          commandManager,
          CommandId::openProject,
          "Open...",
          assetManager.getIcon(
              assets::IconAssetId::openProject));

      result.addColumnBreak();


      result.addCommandItem(
          commandManager,
          CommandId::saveProject,
          "Save Project",
          assetManager.getIcon(
              assets::IconAssetId::saveProject));

      result.addCommandItem(
          commandManager,
          CommandId::saveEdit,
          "Save Edit",
          assetManager.getIcon(
              assets::IconAssetId::saveEdit));

      result.addCommandItem(
          commandManager,
          CommandId::saveAs,
          "Save As...",
          assetManager.getIcon(
              assets::IconAssetId::saveAs));

      result.addCommandItem(
          commandManager,
          CommandId::saveAll,
          "Save All",
          assetManager.getIcon(
              assets::IconAssetId::saveAll));

      result.addCommandItem(
          commandManager,
          CommandId::saveAndQuit,
          "Save and Quit",
          assetManager.getIcon(
              assets::IconAssetId::saveAndQuit));

      result.addColumnBreak();


      result.addCommandItem(
          commandManager,
          CommandId::openSettings,
          "Settings...",
          assetManager.getIcon(
              assets::IconAssetId::settings));

      result.addCommandItem(
          commandManager,
          CommandId::openProjectSettings,
          "Project settings...",
          assetManager.getIcon(
              assets::IconAssetId::projectSettings));

      result.addCommandItem(
          commandManager,
          CommandId::openDeviceManager,
          "Device manager...",
          assetManager.getIcon(
              assets::IconAssetId::deviceManager));

      result.addColumnBreak();


      result.addCommandItem(
          commandManager,
          CommandId::quit,
          "Quit",
          assetManager.getIcon(
              assets::IconAssetId::quit));
      break;


    case 1: // Edit
      result.addCommandItem(
          commandManager,
          CommandId::del,
          "Delete",
          assetManager.getIcon(
              assets::IconAssetId::del));

      result.addCommandItem(
          commandManager,
          CommandId::cut,
          "Cut",
          assetManager.getIcon(
              assets::IconAssetId::cut));

      result.addCommandItem(
          commandManager,
          CommandId::copy,
          "Copy",
          assetManager.getIcon(
              assets::IconAssetId::copy));

      result.addCommandItem(
          commandManager,
          CommandId::paste,
          "Paste",
          assetManager.getIcon(
              assets::IconAssetId::paste));

      result.addColumnBreak();


      result.addCommandItem(
          commandManager,
          CommandId::undo,
          "Undo",
          assetManager.getIcon(
              assets::IconAssetId::undo));

      result.addCommandItem(
          commandManager,
          CommandId::redo,
          "Redo",
          assetManager.getIcon(
              assets::IconAssetId::redo));

      result.addColumnBreak();


      result.addCommandItem(
          commandManager,
          CommandId::addEdit,
          "Add Edit",
          assetManager.getIcon(
              assets::IconAssetId::addEdit));

      result.addCommandItem(
          commandManager,
          CommandId::addTrack,
          "Add Track",
          assetManager.getIcon(
              assets::IconAssetId::addTrack));

      result.addCommandItem(
          commandManager,
          CommandId::addPlugin,
          "Add Plugin",
          assetManager.getIcon(
              assets::IconAssetId::addPlugin));
      break;


    case 2: // View
      result.addCommandItem(
          commandManager,
          CommandId::toggleMasterTrackPanel,
          "Toggle Master Track Panel",
          assetManager.getIcon(
              assets::IconAssetId::toggleMasterTrackPanel));

      result.addCommandItem(
          commandManager,
          CommandId::toggleControlSurfacePanel,
          "Toggle Control Surface Panel",
          assetManager.getIcon(
              assets::IconAssetId::toggleControlSurfacePanel));

      result.addCommandItem(
          commandManager,
          CommandId::toggleBrowserPanel,
          "Toggle Browser Panel",
          assetManager.getIcon(
              assets::IconAssetId::toggleBrowserPanel));
      break;


    case 3: // Help
      result.addCommandItem(
          commandManager,
          CommandId::openHelp,
          "Help me!",
          assetManager.getIcon(
              assets::IconAssetId::help));

      result.addCommandItem(
          commandManager,
          CommandId::openInfo,
          "Info...",
          assetManager.getIcon(
              assets::IconAssetId::info));

      result.addCommandItem(
          commandManager,
          CommandId::openDev,
          "Open Developers Window...",
          assetManager.getIcon(
              assets::IconAssetId::dev));
      break;


    default:
      break;
  }

  return result;
}


// TODO?
[[maybe_unused]] void BodyMenuBar::menuItemSelected(
    int,
    int)
{
}

BLOOPER_NAMESPACE_END
