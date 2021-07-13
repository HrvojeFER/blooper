#include <blooper/internal/utils/selection.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

// Project

bool ProjectSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable& object1,
    te::Selectable& object2)
{
  auto project1 = dynamic_cast<JuceProject*>(std::addressof(object1));
  auto project2 = dynamic_cast<JuceProject*>(std::addressof(object2));

  if (!project1 || !project2) return false;

  return project1->engine
             .getProjectManager()
             .getFolderItemFor(*project1)
             .getParent() ==
         project2->engine
             .getProjectManager()
             .getFolderItemFor(*project2)
             .getParent();
}


void ProjectSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto project = dynamic_cast<JuceProject*>(selectable))
    {
      project->engine.getProjectManager()
          .removeProjectFromList(project->getProjectFile());
    }
  }
}


void ProjectSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool ProjectSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool ProjectSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


// ??
void ProjectSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Edit

bool EditSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable& object1,
    te::Selectable& object2)
{
  auto edit1 = dynamic_cast<JuceEdit*>(std::addressof(object1));
  auto edit2 = dynamic_cast<JuceEdit*>(std::addressof(object2));

  if (!edit1 || !edit2) return false;

  return edit1->engine
             .getProjectManager()
             .getProject(*edit1)
             ->getProjectID() ==
         edit2->engine
             .getProjectManager()
             .getProject(*edit1)
             ->getProjectID();
}


void EditSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto edit = dynamic_cast<JuceEdit*>(selectable))
    {
      edit->engine.getProjectManager().getProject(*edit)->removeProjectItem(
          edit->getProjectItemID(), false);
    }
  }
}


void EditSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool EditSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool EditSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


// ??
void EditSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Track

bool TrackSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable& object1,
    te::Selectable& object2)
{
  auto track1 = dynamic_cast<JuceTrack*>(std::addressof(object1));
  auto track2 = dynamic_cast<JuceTrack*>(std::addressof(object2));

  if (!track1 || !track2) return false;

  return track1->edit.getProjectItemID() ==
         track2->edit.getProjectItemID();
}


void TrackSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto track = dynamic_cast<JuceTrack*>(selectable))
    {
      track->edit.deleteTrack(track);
    }
  }
}


void TrackSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool TrackSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool TrackSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


// ??
void TrackSelectableClass::selectOtherObjects(
    const te::SelectableClass::SelectOtherObjectsParams&)
{
}


// Clip

bool ClipSelectableClass::canObjectsBeSelectedAtTheSameTime(
    te::Selectable& object1,
    te::Selectable& object2)
{
  auto clip1 = dynamic_cast<JuceClip*>(std::addressof(object1));
  auto clip2 = dynamic_cast<JuceClip*>(std::addressof(object2));

  if (!clip1 || !clip2) return false;

  return clip1->getTrack() ==
         clip2->getTrack();
}


void ClipSelectableClass::deleteSelected(
    const te::SelectableList& list,
    bool)
{
  for (auto selectable : list)
  {
    if (auto clip = dynamic_cast<JuceClip*>(selectable))
    {
      clip->removeFromParentTrack();
    }
  }
}


void ClipSelectableClass::addClipboardEntriesFor(
    te::SelectableClass::AddClipboardEntryParams&)
{
}

bool ClipSelectableClass::pasteClipboard(
    const te::SelectableList&,
    int)
{
  return false;
}

bool ClipSelectableClass::canCutSelected(
    const te::SelectableList&)
{
  return false;
}


void ClipSelectableClass::selectOtherObjects(
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

  return plugin1->getOwnerTrack() ==
         plugin2->getOwnerTrack();
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

BLOOPER_DECLARE_SELECTABLE_CLASS(ProjectSelectableClass, JuceProject);
BLOOPER_DECLARE_SELECTABLE_CLASS(EditSelectableClass, JuceEdit);
BLOOPER_DECLARE_SELECTABLE_CLASS(TrackSelectableClass, JuceTrack);
BLOOPER_DECLARE_SELECTABLE_CLASS(ClipSelectableClass, JuceClip);
BLOOPER_DECLARE_SELECTABLE_CLASS(PluginSelectableClass, JucePlugin);
BLOOPER_DECLARE_SELECTABLE_CLASS(ParameterSelectableClass, JuceParameter);

#undef BLOOPER_DECLARE_SELECTABLE_CLASS

BLOOPER_UTIL_NAMESPACE_END
