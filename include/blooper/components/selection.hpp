#ifndef BLOOPER_SELECTION_HPP
#define BLOOPER_SELECTION_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

class EditTrackSelectableClass final : public te::SelectableClass
{
 public:
  bool canObjectsBeSelectedAtTheSameTime(
      te::Selectable& object1,
      te::Selectable& object2) final;


  void addClipboardEntriesFor(
      AddClipboardEntryParams& params) final;

  void deleteSelected(
      const te::SelectableList& list,
      bool                      partOfCutOperation) final;

  bool pasteClipboard(
      const te::SelectableList& currentlySelectedItems,
      int                       editViewID) final;

  bool canCutSelected(
      const te::SelectableList& selectedObjects) final;


  void selectOtherObjects(
      const SelectOtherObjectsParams& params) final;
};

class PluginSelectableClass final : public te::SelectableClass
{
 public:
  bool canObjectsBeSelectedAtTheSameTime(
      te::Selectable& object1,
      te::Selectable& object2) final;


  void addClipboardEntriesFor(
      AddClipboardEntryParams& params) final;

  void deleteSelected(
      const te::SelectableList& list,
      bool                      partOfCutOperation) final;

  bool pasteClipboard(
      const te::SelectableList& currentlySelectedItems,
      int                       editViewID) final;

  bool canCutSelected(
      const te::SelectableList& selectedObjects) final;


  void selectOtherObjects(
      const SelectOtherObjectsParams& params) final;
};

class ParameterSelectableClass final : public te::SelectableClass
{
 public:
  bool canObjectsBeSelectedAtTheSameTime(
      te::Selectable& object1,
      te::Selectable& object2) final;


  void addClipboardEntriesFor(
      AddClipboardEntryParams& params) final;

  void deleteSelected(
      const te::SelectableList& list,
      bool                      partOfCutOperation) final;

  bool pasteClipboard(
      const te::SelectableList& currentlySelectedItems,
      int                       editViewID) final;

  bool canCutSelected(
      const te::SelectableList& selectedObjects) final;


  void selectOtherObjects(
      const SelectOtherObjectsParams& params) final;
};

BLOOPER_UTIL_NAMESPACE_END

#endif //BLOOPER_SELECTION_HPP
