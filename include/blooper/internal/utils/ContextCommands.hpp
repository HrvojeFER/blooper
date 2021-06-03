#ifndef BLOOPER_CONTEXT_COMMANDS_HPP
#define BLOOPER_CONTEXT_COMMANDS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_NAMESPACE_BEGIN

enum class CommandId : JuceCommandIdUnderlyingType
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

  addTrack [[maybe_unused]],

  deleteTrack [[maybe_unused]]
};


[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    CommandId            commandId)
{
  const auto id = static_cast<std::underlying_type_t<CommandId>>(commandId);

  context.getCommandManager().invokeDirectly(id, true);
}

[[maybe_unused]] inline void dispatch(
    AbstractCoreContext& context,
    JuceCommand          command)
{
  context.getCommandManager().invoke(move(command), true);
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_COMMANDS_HPP
