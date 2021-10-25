#ifndef BLOOPER_EXT_PROJECT_HPP
#define BLOOPER_EXT_PROJECT_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline bool hasEdit(te::Project& project)
{
  for (int i = 0; i < project.getNumProjectItems(); ++i)
    if (project.getProjectItemAt(i)->isEdit())
      return true;

  return false;
}

[[maybe_unused]] inline std::unique_ptr<te::Edit> getEdit(
    te::Project& project,
    te::Engine&  engine)
{
  for (int i = 0; i < project.getNumProjectItems(); ++i)
  {
    auto item = project.getProjectItemAt(i);
    if (item->isEdit())
      return te::loadEditFromFile(
          engine,
          item->getSourceFile());
  }

  return nullptr;
}

[[maybe_unused]] inline std::unique_ptr<te::Edit> ensureEdit(
    te::Project& project,
    te::Engine&  engine)
{
  if (auto edit = getEdit(project, engine))
    return edit;

  return te::loadEditFromFile(
      engine,
      project.createNewEdit()->getSourceFile());
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_PROJECT_HPP
