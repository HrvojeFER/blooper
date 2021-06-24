#include <blooper/internal/utils/selection.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>
#include <blooper/internal/utils/EditTrack.hpp>

#include <blooper/context/behaviour/EditManager.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

// EditTrack

bool EditTrackSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable&,
    te::Selectable&)
{
  return true;
}


void EditTrackSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto editTrack = dynamic_cast<EditTrack*>(selectable))
    {
      editTrack->getContext().getEditManager().remove(editTrack->getId());
    }
  }
}


void EditTrackSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool EditTrackSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool EditTrackSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


// ??
void EditTrackSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Plugin

bool PluginSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable& object1,
    te::Selectable& object2)
{
  auto plugin1 = dynamic_cast<JucePlugin*>(std::addressof(object1));
  auto plugin2 = dynamic_cast<JucePlugin*>(std::addressof(object2));

  if (!plugin1 || !plugin2) return false;

  return plugin1->getOwnerTrack() == plugin2->getOwnerTrack();
}


void PluginSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto plugin = dynamic_cast<JucePlugin*>(selectable))
    {
      plugin->deleteFromParent();
    }
  }
}


void PluginSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool PluginSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool PluginSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


void PluginSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Parameter

bool ParameterSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable&,
    te::Selectable&)
{
  return true;
}

void ParameterSelectableClass::deleteSelected(
    const te::SelectableList&,
    bool)
{
}


void ParameterSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool ParameterSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool ParameterSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


// ??
void ParameterSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Declarations

#define BLOOPER_DECLARE_SELECTABLE_CLASS(_class, _object) \
  te::SelectableClass::ClassInstance<                     \
      _class,                                             \
      _object> /* NOLINTNEXTLINE(cert-err58-cpp) */       \
      _class##Instance

BLOOPER_DECLARE_SELECTABLE_CLASS(EditTrackSelectableClass, EditTrack);
BLOOPER_DECLARE_SELECTABLE_CLASS(PluginSelectableClass, JucePlugin);
BLOOPER_DECLARE_SELECTABLE_CLASS(ParameterSelectableClass, JuceParameter);

#undef BLOOPER_DECLARE_SELECTABLE_CLASS

BLOOPER_UTIL_NAMESPACE_END
