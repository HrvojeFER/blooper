#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditManager::EditManager(AbstractContext& context)
    : ContextualBase(context),
      StatefulBase(
          this->getContext()
              .getProjectState()
              .getOrCreateChildWithName(
                  id::edit,
                  nullptr))
{
  auto& project = this->getContext().getProject();
  auto  undoManager = std::addressof(this->getContext().getUndoManager());


  for (int i = 0; i < project.getNumProjectItems(); ++i)
    if (JuceProjectItemRef item = project.getProjectItemAt(i); item->isEdit())
      this->add(move(item), nullptr);

  this->soloed.referTo(
      this->getState(),
      id::soloed,
      undoManager,
      EditTrack::invalidId);
  if (this->isAnyTrackSoloed())
  {
    this->visit([this](EditTrack* track) {
      if (track->id == this->soloed)
        track->audio->setMute(false);
      else
        track->audio->setMute(true);
    });
  }
}

EditManager::~EditManager() = default;


EditTrackRef EditManager::add()
{
  return this->add(
      this->getContext().getProject().createNewEdit(),
      std::addressof(this->getContext().getUndoManager()));
}

void EditManager::remove(EditTrack::Id id)
{
  auto undoManager = std::addressof(this->getContext().getUndoManager());

  this->edits.erase(id);

  this->getState().removeChild(
      this->getState().getChildWithProperty(
          te::IDs::uid,
          id),
      undoManager);
}

EditTrackRef EditManager::get(EditTrack::Id id)
{
  return this->edits[id];
}


EditTrackRef EditManager::add(
    JuceProjectItemRef item,
    JuceUndoManager*   undoManager)
{
  BLOOPER_ASSERT(item->isEdit());
  auto id = getEditTrackId(*item);

  auto editChild =
      this->getState()
          .getChildWithProperty(
              te::IDs::uid,
              id);
  if (!editChild.isValid())
  {
    editChild = JuceState{id::edit};
    editChild.setProperty(te::IDs::uid, id, nullptr);

    auto track =
        new EditTrack(
            this->getContext(),
            editChild,
            move(item));

    this->edits.emplace(id, track);
    this->getState().appendChild(move(editChild), undoManager);
    return track;
  }
  else
  {
    auto track =
        new EditTrack(
            this->getContext(),
            move(editChild),
            move(item));

    this->edits.emplace(id, track);
    return track;
  }
}


// ValueTreeListener

void EditManager::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& _id)
{
  if (tree == this->getState())
  {
    if (_id == id::soloed)
    {
      if (this->isAnyTrackSoloed())
      {
        this->visit([this](EditTrack* track) {
          if (track->id == this->soloed)
            track->audio->setMute(false);
          else
            track->audio->setMute(true);
        });
      }
      else
      {
        this->visit([](EditTrack* track) {
          track->audio->setMute(track->muted);
        });
      }
    }
  }
}

BLOOPER_NAMESPACE_END
