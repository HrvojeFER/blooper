#ifndef BLOOPER_EXT_EDIT_HPP
#define BLOOPER_EXT_EDIT_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Load

[[maybe_unused]] inline std::shared_ptr<te::Edit> loadEditFromItem(
    te::Engine&      engine,
    te::ProjectItem& item)
{
  BLOOPER_ASSERT(item.isEdit());

  std::shared_ptr edit{
      te::loadEditFromFile(
          engine,
          item.getSourceFile())};

  edit->setProjectItemID(item.getID());

  return edit;
}


// Visit

template<typename TCallback>
[[maybe_unused]] inline void visit(te::Edit& edit, TCallback callback)
{
  static_assert(
      isInvokable
      );
}


// Armed

[[maybe_unused]] inline juce::Array<te::Track*> getArmedTracks(te::Edit& edit);


// Transport

[[maybe_unused]] inline bool isPlaying(te::Edit& edit)
{
  return edit.getTransport().isPlaying();
}

[[maybe_unused]] inline void togglePlaying(te::Edit& edit);


[[maybe_unused]] inline bool isRecording(te::Edit& edit)
{
  return edit.getTransport().isRecording();
}

[[maybe_unused]] void toggleRecording(te::Edit& edit);

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_EDIT_HPP
