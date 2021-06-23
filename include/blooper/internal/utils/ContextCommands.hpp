#ifndef BLOOPER_CONTEXT_COMMANDS_HPP
#define BLOOPER_CONTEXT_COMMANDS_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

namespace CommandId
{
enum _ : JuceCommandId
{
  // Special

  none [[maybe_unused]] = -1,


  // App

  quit [[maybe_unused]] = 1000,
  save [[maybe_unused]] = 1001,
  saveAndQuit [[maybe_unused]] = 1002,
  saveAll [[maybe_unused]] = 1003,
  saveAs [[maybe_unused]] = 1004,

  openSettings [[maybe_unused]] = 1100,
  openProjectSettings [[maybe_unused]] = 1101,
  openDeviceManager [[maybe_unused]] = 1102,

  openProject [[maybe_unused]] = 1200,

  openHelp [[maybe_unused]] = 1300,
  openInfo [[maybe_unused]] = 1301,
  openDev [[maybe_unused]] = 1302,

  toggleMasterTrackPanel [[maybe_unused]] = 1400,
  toggleControlSurfacePanel [[maybe_unused]] = 1401,
  toggleBrowserPanel [[maybe_unused]] = 1402,


  // Edit

  del [[maybe_unused]] = 2000,
  cut [[maybe_unused]] = 2001,
  copy [[maybe_unused]] = 2002,
  paste [[maybe_unused]] = 2003,

  addTrack [[maybe_unused]] = 2100,
  addPlugin [[maybe_unused]] = 2101,

  undo [[maybe_unused]] = 2200,
  redo [[maybe_unused]] = 2201,

  selectAll [[maybe_unused]] = 2300,
  deselectAll [[maybe_unused]] = 2301,


  // Transport

  play [[maybe_unused]] = 3000,
  pause [[maybe_unused]] = 3001,
  stop [[maybe_unused]] = 3002,
  record [[maybe_unused]] = 3003,

  toggleMonitoring [[maybe_unused]] = 3100,


  // Track

  muteTrack [[maybe_unused]] = 4001,
  soloTrack [[maybe_unused]] = 4002,
  armTrack [[maybe_unused]] = 4003,

  cycleTrackMode [[maybe_unused]] = 4200,

  clearTrack [[maybe_unused]] = 4300,


  // Parameter

  nudgeUp [[maybe_unused]] = 5000,
  nudgeDown [[maybe_unused]] = 5001,

  elevateParameter [[maybe_unused]] = 5100,
};
}

[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    JuceCommandId        commandId)
{
  if (commandId == CommandId::none) return;

  context.getCommandManager()
      .invokeDirectly(
          commandId,
          true);
}

[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    JuceCommand          command)
{
  if (command.commandID == -1) return;

  context.getCommandManager()
      .invoke(
          move(command),
          true);
}


template<typename... TCommandIds>
[[maybe_unused]] inline void fillCommands(
    juce::Array<JuceCommandId>& in,
    TCommandIds... ids)
{
  static_assert(
      meta::and_(
          meta::true_c,
          meta::traits::is_castable(
              meta::typeid_(ids),
              meta::type_c<JuceCommandId>)...),
      "Ids passed to addCommands must be castable to JuceCommandId.");

  (in.add(static_cast<JuceCommandId>(ids)), ...);
}

[[maybe_unused]] inline void fillCommandInfo(
    JuceCommandInfo& in,
    JuceCommandId    commandId)
{
  in.commandID = commandId;

  switch (commandId)
  {
      // Special

    case CommandId::none:
      in.categoryName = "General";
      in.shortName = "None";
      in.description = "No-op.";
      break;


      // App

    case CommandId::quit:
      in.categoryName = "App";
      in.shortName = "Quit";
      in.description = "Quits blooper.";
      break;

    case CommandId::save:
      in.categoryName = "App";
      in.shortName = "Save";
      in.description = "Save project.";
      break;

    case CommandId::saveAndQuit:
      in.categoryName = "App";
      in.shortName = "Save And Quit";
      in.description = "Save project and quit blooper.";
      break;

    case CommandId::saveAll:
      in.categoryName = "App";
      in.shortName = "Save All";
      in.description = "Save project and settings.";
      break;

    case CommandId::saveAs:
      in.categoryName = "App";
      in.shortName = "Save As...";
      in.description = "Save project in a specified location.";
      break;


    case CommandId::openSettings:
      in.categoryName = "App";
      in.shortName = "Open Settings";
      in.description = "Open the Settings window.";
      break;

    case CommandId::openProjectSettings:
      in.categoryName = "App";
      in.shortName = "Open Settings";
      in.description = "Open the Project Settings window.";
      break;

    case CommandId::openDeviceManager:
      in.categoryName = "App";
      in.shortName = "Open Device Manager";
      in.description = "Open the Device Manager window.";
      break;


    case CommandId::openProject:
      in.categoryName = "App";
      in.shortName = "Open Project...";
      in.description = "Unload the current project and open a new one.";
      break;


    case CommandId::openHelp:
      in.categoryName = "App";
      in.shortName = "Open Help";
      in.description = "Open the Help window.";
      break;

    case CommandId::openInfo:
      in.categoryName = "App";
      in.shortName = "Open Info";
      in.description = "Open the Info window.";
      break;

    case CommandId::openDev:
      in.categoryName = "App";
      in.shortName = "Open Dev";
      in.description = "Open the Developers window.";
      break;


    case CommandId::toggleMasterTrackPanel:
      in.categoryName = "App";
      in.shortName = "Toggle Master Track Panel";
      in.description = "Toggle Master Track panel view.";
      break;

    case CommandId::toggleControlSurfacePanel:
      in.categoryName = "App";
      in.shortName = "Toggle Control Surface Panel";
      in.description = "Toggle Control Surface panel view.";
      break;

    case CommandId::toggleBrowserPanel:
      in.categoryName = "App";
      in.shortName = "Toggle Browser Panel";
      in.description = "Toggle Browser panel view.";
      break;


      // Edit

    case CommandId::del:
      in.categoryName = "Edit";
      in.shortName = "Delete";
      in.description = "Delete selection.";
      break;

    case CommandId::cut:
      in.categoryName = "Edit";
      in.shortName = "Cut";
      in.description = "Cut selection.";
      break;

    case CommandId::copy:
      in.categoryName = "Edit";
      in.shortName = "Copy";
      in.description = "Copy selection.";
      break;

    case CommandId::paste:
      in.categoryName = "Edit";
      in.shortName = "Paste";
      in.description = "Paste clipboard to selection.";
      break;


    case CommandId::addTrack:
      in.categoryName = "Edit";
      in.shortName = "Add Track";
      in.description = "Add a new Track.";
      break;

    case CommandId::addPlugin:
      in.categoryName = "Edit";
      in.shortName = "Plugin";
      in.description = "Add a new Plugin to the selected Tracks.";
      break;


    case CommandId::undo:
      in.categoryName = "Edit";
      in.shortName = "Undo";
      in.description = "Undo last undoable action.";
      break;

    case CommandId::redo:
      in.categoryName = "Edit";
      in.shortName = "Redo";
      in.description = "Redo last undoable action.";
      break;


    case CommandId::selectAll:
      in.categoryName = "Edit";
      in.shortName = "Select All";
      in.description = "Select everything Selectable in current Component.";
      break;

    case CommandId::deselectAll:
      in.categoryName = "Edit";
      in.shortName = "Deselect All";
      in.description = "Deselect all selections.";
      break;


      // Transport

    case CommandId::play:
      in.categoryName = "Transport";
      in.shortName = "Play";
      in.description = "Starts the blooper loop.";
      break;

    case CommandId::pause:
      in.categoryName = "Transport";
      in.shortName = "Pause";
      in.description = "Pauses the blooper loop.";
      break;

    case CommandId::stop:
      in.categoryName = "Transport";
      in.shortName = "Stop";
      in.description = "Stops the blooper loop.";
      break;

    case CommandId::record:
      in.categoryName = "Transport";
      in.shortName = "Record";
      in.description = "Records input into armed tracks.";
      break;


    case CommandId::toggleMonitoring:
      in.categoryName = "Transport";
      in.shortName = "Toggle Monitoring";
      in.description = "Toggle Monitoring of all inputs.";
      break;


      // Track

    case CommandId::muteTrack:
      in.categoryName = "Track";
      in.shortName = "Mute Tracks";
      in.description = "Mute selected Tracks.";
      break;

    case CommandId::soloTrack:
      in.categoryName = "Track";
      in.shortName = "Solo Tracks";
      in.description = "Solo selected Tracks.";
      break;

    case CommandId::armTrack:
      in.categoryName = "Track";
      in.shortName = "Arm Tracks";
      in.description = "Arm selected Tracks.";
      break;


    case CommandId::cycleTrackMode:
      in.categoryName = "Track";
      in.shortName = "Cycle Track Mode";
      in.description = "Cycle Track Mode of the first Track in Selection.";
      break;


    case CommandId::clearTrack:
      in.categoryName = "Track";
      in.shortName = "Clear Track";
      in.description = "Clear tracks in selection.";
      break;


      // Parameter

    case CommandId::nudgeUp:
      in.categoryName = "Parameter";
      in.shortName = "Nudge Up";
      in.description = "Nudge up the controls in selection.";
      break;

    case CommandId::nudgeDown:
      in.categoryName = "Parameter";
      in.shortName = "Nudge Down";
      in.description = "Nudge down the controls in selection.";
      break;


    default:
      break;
  }
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_COMMANDS_HPP
