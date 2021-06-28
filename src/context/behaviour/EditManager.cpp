#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/internal/abstract/id.hpp>

BLOOPER_NAMESPACE_BEGIN

EditManager::EditManager(AbstractContext& context)
    : ContextualBase(context),
      StatefulBase(
          this->getContext()
              .getProjectState()
              .getOrCreateChildWithName(
                  id::edit,
                  nullptr)),

      inputsUpdate(false),
      soloedUpdate(false)
{
  auto& project = this->getContext().getProject();
  auto  undoManager = std::addressof(this->getContext().getUndoManager());


  this->soloed.referTo(
      this->getState(),
      id::soloed,
      undoManager,
      EditTrack::invalidId);


  for (int i = 0; i < project.getNumProjectItems(); ++i)
    if (JuceProjectItemRef item = project.getProjectItemAt(i); item->isEdit())
      this->add(move(item), nullptr);


  this->updateInputs();
  this->updateSoloed();
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
  auto& project = this->getContext().getProject();

  project.removeProjectItem(
      te::ProjectItemID{id, project.getProjectID()},
      false);

  this->getState()
      .removeChild(
          this->getState().getChildWithProperty(
              te::IDs::uid,
              id),
          this->getContext().getUndoManagerPtr());

  this->edits.erase(id);
}

EditTrackRef EditManager::get(EditTrack::Id id)
{
  return this->edits[id];
}


void EditManager::updateInputs() const
{
  this->visit([index = 0](EditTrack* track) mutable {
    for (auto inputInstance : track->getEdit().getAllInputDevices())
    {
      if (inputInstance->getInputDevice().getDeviceType() ==
          te::InputDevice::waveDevice)
      {
        inputInstance->setTargetTrack(
            track->getAudio(),
            0,
            true);

        inputInstance->setRecordingEnabled(
            track->getAudio(),
            track->armed);
      }
    }
  });
}

void EditManager::updateSoloed()
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
      this->markAndUpdate(this->soloedUpdate);
    }
  }
}

void EditManager::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->getState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->inputsUpdate);
    }
  }
}

void EditManager::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->getState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->inputsUpdate);
    }
  }
}

void EditManager::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int,
    int)
{
  if (tree == this->getState())
  {
    this->markAndUpdate(this->inputsUpdate);
  }
}


// FlaggedAsyncUpdater

void EditManager::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(
          this->inputsUpdate))
  {
    this->updateInputs();
  }
  else if (util::FlaggedAsyncUpdater::compareAndReset(
               this->soloedUpdate))
  {
    this->updateSoloed();
  }
}

BLOOPER_NAMESPACE_END
