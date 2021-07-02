#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/edit.hpp>
#include <blooper/internal/ext/track.hpp>

BLOOPER_NAMESPACE_BEGIN

EditManager::EditManager(
    AbstractContext& context,
    State            state)
    : ContextualBase(context),
      StatefulBase(move(state)),

      inputsUpdate(false),
      bpmUpdate(false),

      masterEdit(),
      masterTransport(),
      masterTempo(),
      masterBpmSetting(),
      masterTimeSigSetting()
{
  this->bpm.referTo(
      this->getContext().getProjectSettings(),
      id::bpm,
      this->getContext().getUndoManagerPtr(),
      defaultBpm);

  this->focusedEdit.referTo(
      this->getState(),
      id::focusedEdit,
      nullptr,
      0);

  this->ensureMasterEdit();

  auto& project = this->getContext().getProject();

  for (int i = 0; i < project.getNumProjectItems(); ++i)
    if (JuceProjectItemRef item = project.getProjectItemAt(i);
        item->isEdit() && !this->isMasterEdit(item->getID().getItemID()))
      this->add(*item, nullptr);

  this->updateInputs();
  this->updateBpm();

  this->getContext().getProjectSettings().addListener(this);
}

EditManager::~EditManager()
{
  this->getContext().getProjectSettings().removeListener(this);
}


JuceEditRef EditManager::get(int id)
{
  return this->edits.at(id);
}

JuceEditConstRef EditManager::get(int id) const
{
  return this->edits.at(id);
}


JuceEditRef EditManager::add()
{
  return this->add(
      *this->getContext().getProject().createNewEdit(),
      std::addressof(this->getContext().getUndoManager()));
}

void EditManager::remove(int id)
{
  auto& project = this->getContext().getProject();

  project.removeProjectItem(
      JuceProjectItemId{
          id,
          project.getProjectID()},
      false);

  this->getState()
      .removeChild(
          this->getState().getChildWithProperty(
              te::IDs::uid,
              id),
          this->getContext().getUndoManagerPtr());

  this->edits.erase(id);
}


void EditManager::ensureMasterEdit()
{
  auto& context = this->getContext();
  auto& project = context.getProject();

  auto masterEditChild =
      this->getState()
          .getChildWithName(id::masterEdit);
  JuceProjectItemRef masterEditItem{};

  if (!masterEditChild.isValid())
  {
    masterEditChild = JuceState{id::masterEdit};
    masterEditItem = project.createNewEdit();

    masterEditChild.setProperty(
        te::IDs::uid,
        masterEditItem->getID().getItemID(),
        nullptr);

    this->getState().addChild(
        move(masterEditChild),
        -1,
        context.getUndoManagerPtr());
  }
  else
  {
    masterEditItem =
        project.getProjectItemForID(
            JuceProjectItemId{
                masterEditChild.getProperty(te::IDs::uid),
                project.getProjectID()});
  }

  this->masterEdit =
      ext::loadEditFromItem(
          context.getEngine(),
          *masterEditItem);

  this->masterTransport = std::addressof(this->masterEdit->getTransport());

  this->masterTempo = std::addressof(this->masterEdit->tempoSequence);
  this->masterTempo->createEmptyState();

  this->masterBpmSetting = this->masterTempo->insertTempo(0.0);
  this->masterBpmSetting->setBpm(this->bpm);

  this->masterTimeSigSetting = this->masterTempo->insertTimeSig(0.0);
  this->masterTimeSigSetting->numerator = 4;
  this->masterTimeSigSetting->denominator = 4;

  this->masterTransport->setCurrentPosition(0.0);
  this->masterTransport->looping = true;
  this->masterTransport->setLoopPoint1(0.0);
  this->masterTransport->setLoopPoint2(
      this->masterTempo->beatsToTime(16));

  this->masterTransport->play(false);
}

bool EditManager::isMasterEdit(int id) const
{
  auto masterEditChild =
      this->getState()
          .getChildWithProperty(
              te::IDs::uid,
              id);

  if (!masterEditChild.isValid()) return false;

  return masterEditChild.getProperty(id::masterEdit);
}


void EditManager::updateInputs() const
{
  this->visit([this](JuceEdit& edit) {
    this->updateInputs(edit);
  });
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void EditManager::updateInputs(JuceEdit& edit) const
{
  edit.visitAllTopLevelTracks(
      [&edit, index = 0](JuceTrack& track) mutable {
        if (auto* audioTrack =
                dynamic_cast<te::AudioTrack*>(
                    std::addressof(track)))
        {
          for (auto inputInstance : edit.getAllInputDevices())
          {
            if (inputInstance->getInputDevice().getDeviceType() ==
                te::InputDevice::waveDevice)
            {
              inputInstance->setTargetTrack(
                  *audioTrack,
                  index,
                  true);

              inputInstance->setRecordingEnabled(
                  *audioTrack,
                  ext::isArmed(*audioTrack));
            }
          }
        }

        index++;
        return true; // continue to other tracks
      });
}


void EditManager::updateBpm() const
{
  this->masterBpmSetting->setBpm(this->bpm);
  this->masterTransport->setLoopPoint2(
      this->masterTempo->beatsToTime(16));

  this->visit([this](JuceEdit& edit) {
    this->updateBpm(edit);
  });
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void EditManager::updateBpm(JuceEdit& edit) const
{
  edit.tempoSequence.getTempo(0)->setBpm(this->bpm);
}


JuceEditRef EditManager::add(
    JuceProjectItem& item,
    JuceUndoManager* undoManager)
{
  BLOOPER_ASSERT(item.isEdit());
  const auto id = item.getID().getItemID();

  auto edit =
      ext::loadEditFromItem(
          this->getContext().getEngine(),
          item);
  this->edits.emplace(id, edit);
  edit->getTransport()
      .syncToEdit(
          this->masterEdit.get(),
          false);

  edit->tempoSequence.createEmptyState();
  auto tempo = edit->tempoSequence.insertTempo(0.0);
  tempo->setBpm(this->bpm);
  auto timeSig = edit->tempoSequence.insertTimeSig(0.0);
  timeSig->denominator = 4;
  timeSig->numerator = 4;

  edit->getTransport().play(false);


  auto editChild =
      this->getState()
          .getChildWithProperty(
              te::IDs::uid,
              id);
  if (!editChild.isValid())
  {
    editChild = JuceState{id::edit};
    editChild.setProperty(
        te::IDs::uid,
        id,
        nullptr);

    this->getState().appendChild(move(editChild), undoManager);
  }

  return std::move(edit);
}


// ValueTreeListener

void EditManager::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getContext().getProjectSettings())
  {
    if (id == id::bpm)
    {
      this->markAndUpdate(this->bpmUpdate);
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
  if (util::FlaggedAsyncUpdater::compareAndReset(this->inputsUpdate))
  {
    this->updateInputs();
  }

  if (util::FlaggedAsyncUpdater::compareAndReset(this->bpmUpdate))
  {
    this->updateBpm();
  }
}

BLOOPER_NAMESPACE_END
