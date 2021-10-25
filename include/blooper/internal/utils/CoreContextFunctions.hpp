#ifndef BLOOPER_CORE_CONTEXT_FUNCTIONS_HPP
#define BLOOPER_CORE_CONTEXT_FUNCTIONS_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/contextual.hpp>
#include <blooper/internal/ext/value_tree.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline auto getProjectPath(
    AbstractCoreContext& context,
    const JuceProject&   project)
{
  auto& manager = context.getEngine().getProjectManager();

  JuceStringArray result;

  ext::visitAncestors(
      manager.getFolderItemFor(project),
      [&result](const juce::ValueTree& node) mutable {
        if (node.hasType(te::IDs::FOLDER))
        {
          result.add(node.getProperty(te::IDs::name));
        }
      });

  return move(result);
}

[[maybe_unused]] inline auto addProjectFolder(
    AbstractCoreContext&   context,
    const JuceStringArray& path)
{
  auto& manager = context.getEngine().getProjectManager();

  auto currentFolder = manager.folders;
  for (const auto& folderName : path)
  {
    manager.createNewProjectFolder(
        currentFolder,
        folderName);

    currentFolder =
        currentFolder.getChildWithProperty(
            te::IDs::name,
            folderName);
  }

  return move(currentFolder);
}

[[maybe_unused]] inline void deleteProjectFolder(
    AbstractCoreContext&   context,
    const JuceStringArray& path)
{
  auto& manager = context.getEngine().getProjectManager();

  auto currentFolder = manager.folders;
  for (const auto& folderName : path)
  {
    if (auto childFolder =
            currentFolder.getChildWithProperty(
                te::IDs::name,
                folderName);
        childFolder.isValid() && childFolder.hasType(te::IDs::FOLDER))
      currentFolder = childFolder;
    else
      return;
  }

  manager.deleteProjectFolder(currentFolder);
}

[[maybe_unused]] inline auto addProject(
    AbstractCoreContext&   context,
    const JuceStringArray& path,
    const JuceString&      name)
{
  auto& manager = context.getEngine().getProjectManager();

  for (auto openProject : manager.getAllProjects(manager.folders))
    if (name == openProject->getName())
      return JuceProjectRef(openProject);

  auto folder = addProjectFolder(context, path);
  auto project = manager.createNewProjectInteractively(
      name,
      context.getProjectsDir(),
      folder);

  return move(project);
}

[[maybe_unused]] inline void deleteProject(
    AbstractCoreContext& context,
    JuceProjectRef       project)
{
  auto& manager = context.getEngine().getProjectManager();

  if (auto projectNode =
          ext::find(
              manager.folders,
              [project = move(project)](const auto& node) {
                return node[te::IDs::file].toString() ==
                       project->getProjectFile().getFullPathName();
              });
      projectNode.isValid())
  {
    manager.deleteProjectFolder(projectNode);
  }
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CORE_CONTEXT_FUNCTIONS_HPP
