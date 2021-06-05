#ifndef BLOOPER_CONTEXT_COMMANDS_HPP
#define BLOOPER_CONTEXT_COMMANDS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_NAMESPACE_BEGIN

namespace CommandId
{
enum _ : JuceCommandId
{
  // These are commands copied from JUCE

  /** This command ID should be used to send a "Quit the App" command.

      This command is recognised by the JUCEApplication class, so if it is invoked
      and no other ApplicationCommandTarget handles the event first, the JUCEApplication
      object will catch it and call JUCEApplicationBase::systemRequestedQuit().
  */
  quit [[maybe_unused]] = 0x1001,

  /** The command ID that should be used to send a "Delete" command. */
  del [[maybe_unused]] = 0x1002,

  /** The command ID that should be used to send a "Cut" command. */
  cut [[maybe_unused]] = 0x1003,

  /** The command ID that should be used to send a "Copy to clipboard" command. */
  copy [[maybe_unused]] = 0x1004,

  /** The command ID that should be used to send a "Paste from clipboard" command. */
  paste [[maybe_unused]] = 0x1005,

  /** The command ID that should be used to send a "Select all" command. */
  selectAll [[maybe_unused]] = 0x1006,

  /** The command ID that should be used to send a "Deselect all" command. */
  deselectAll [[maybe_unused]] = 0x1007,

  /** The command ID that should be used to send a "undo" command. */
  undo [[maybe_unused]] = 0x1008,

  /** The command ID that should be used to send a "redo" command. */
  redo [[maybe_unused]] = 0x1009,


  // These are blooper commands

  none [[maybe_unused]] = -1,

  play [[maybe_unused]] = 1,
  pause [[maybe_unused]] = 2,
  stop [[maybe_unused]] = 3,
  record [[maybe_unused]] = 4,
  nudgeUp [[maybe_unused]] = 5,
  nudgeDown [[maybe_unused]] = 6,

  muteTrack [[maybe_unused]] = 7,
  soloTrack [[maybe_unused]] = 8,
  armTrack [[maybe_unused]] = 9,
  cycleTrackMode [[maybe_unused]] = 10,
  addTrack [[maybe_unused]] = 11,
  addPlugin [[maybe_unused]] = 12,

  toggleMasterTrack [[maybe_unused]] = 13,
  toggleControlSurface [[maybe_unused]] = 14,
  toggleBrowser [[maybe_unused]] = 15,
};
}

[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    JuceCommandId        commandId)
{
  if (commandId == CommandId::none) return;

  context.getCommandManager().invokeDirectly(commandId, true);
}

[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    JuceCommand          command)
{
  if (command.commandID == -1) return;

  context.getCommandManager().invoke(move(command), true);
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
    case CommandId::none:
      in.categoryName = "General";
      in.shortName = "None";
      in.description = "No-op.";
      break;


    case CommandId::quit:
      in.categoryName = "General";
      in.shortName = "Quit";
      in.description = "Quits blooper.";
      break;


    case CommandId::del:
      in.categoryName = "General";
      in.shortName = "Delete";
      in.description = "Delete selection.";
      break;

    case CommandId::cut:
      in.categoryName = "General";
      in.shortName = "Cut";
      in.description = "Cut selection.";
      break;

    case CommandId::copy:
      in.categoryName = "General";
      in.shortName = "Copy";
      in.description = "Copy selection.";
      break;

    case CommandId::paste:
      in.categoryName = "General";
      in.shortName = "Paste";
      in.description = "Paste clipboard to selection.";
      break;


    case CommandId::undo:
      in.categoryName = "General";
      in.shortName = "Undo";
      in.description = "Undo last undoable action.";
      break;

    case CommandId::redo:
      in.categoryName = "General";
      in.shortName = "Redo";
      in.description = "Redo last undoable action.";
      break;


    case CommandId::play:
      in.categoryName = "Control";
      in.shortName = "Play";
      in.description = "Starts the blooper loop.";
      break;

    case CommandId::pause:
      in.categoryName = "Control";
      in.shortName = "Pause";
      in.description = "Pauses the blooper loop.";
      break;

    case CommandId::stop:
      in.categoryName = "Control";
      in.shortName = "Stop";
      in.description = "Stops the blooper loop.";
      break;

    case CommandId::record:
      in.categoryName = "Control";
      in.shortName = "Record";
      in.description = "Records input into armed tracks.";
      break;

    case CommandId::nudgeUp:
      in.categoryName = "Control";
      in.shortName = "Nudge Up";
      in.description = "Nudge up the controls in selection.";
      break;

    case CommandId::nudgeDown:
      in.categoryName = "Control";
      in.shortName = "Nudge Down";
      in.description = "Nudge down the controls in selection.";
      break;


    case CommandId::muteTrack:
      in.categoryName = "Tracks";
      in.shortName = "Mute Tracks";
      in.description = "Mute selected tracks.";
      break;

    case CommandId::soloTrack:
      in.categoryName = "Tracks";
      in.shortName = "Solo Tracks";
      in.description = "Solo selected tracks.";
      break;

    case CommandId::armTrack:
      in.categoryName = "Tracks";
      in.shortName = "Arm Tracks";
      in.description = "Arm selected tracks.";
      break;

    case CommandId::cycleTrackMode:
      in.categoryName = "Tracks";
      in.shortName = "Cycle Track mode";
      in.description = "Cycle selected track mode.";
      break;

    case CommandId::addTrack:
      in.categoryName = "Control";
      in.shortName = "Add track";
      in.description = "Add track.";
      break;

    case CommandId::addPlugin:
      in.categoryName = "Tracks";
      in.shortName = "Add Plugin";
      in.description = "Add new plugin to the selected tracks.";
      break;


    case CommandId::toggleMasterTrack:
      in.categoryName = "Views";
      in.shortName = "Toggle Master Track";
      in.description = "Toggle Master Track panel view.";
      break;

    case CommandId::toggleControlSurface:
      in.categoryName = "Views";
      in.shortName = "Toggle Control Surface";
      in.description = "Toggle Control Surface panel view.";
      break;

    case CommandId::toggleBrowser:
      in.categoryName = "Views";
      in.shortName = "Toggle Browser";
      in.description = "Toggle Browser panel view.";
      break;


    default:
      break;
  }
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_COMMANDS_HPP
