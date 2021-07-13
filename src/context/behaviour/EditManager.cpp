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

  this->ensureAtLeastOneEdit();


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
  auto edit = this->add(
      *this->getContext().getProject().createNewEdit(),
      this->getContext().getUndoManagerPtr());

  this->focusedEdit = edit->getProjectItemID().getItemID();

  return std::move(edit);
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


  this->ensureAtLeastOneEdit();

  if (id == this->focusedEdit)
    this->focusedEdit = this->edits.begin()->first;
}


void EditManager::ensureMasterEdit()
{
  auto& _context = this->getContext();
  auto& project = _context.getProject();


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
        _context.getUndoManagerPtr());
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
          _context.getEngine(),
          *masterEditItem);


  this->masterTransport = std::addressof(this->masterEdit->getTransport());
  this->masterTransport->ensureContextAllocated();

  this->masterTempo = std::addressof(this->masterEdit->tempoSequence);
  this->masterTempo->createEmptyState();

  this->masterBpmSetting =
      this->masterTempo->insertTempo(tempoStartTime);
  this->masterBpmSetting->setBpm(this->bpm);

  this->masterTimeSigSetting =
      this->masterTempo->insertTimeSig(tempoStartTime);
  this->masterTimeSigSetting->numerator = timeSigNumerator;
  this->masterTimeSigSetting->denominator = timeSigDenominator;

  this->masterTransport->looping = true;
  this->masterTransport->setLoopPoint1(
      this->masterTempo->beatsToTime(loopStartBeat));
  this->masterTransport->setLoopPoint2(
      this->masterTempo->beatsToTime(loopEndBeat));

  ext::setPositionBeats(*this->masterTransport, loopStartBeat);

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

  return masterEditChild.hasType(id::masterEdit);
}


void EditManager::ensureAtLeastOneEdit()
{
  auto hasAtLeastOneEdit = false;
  this->visit(
      [this, &hasAtLeastOneEdit](const te::Edit& edit) {
        if (!this->isMasterEdit(edit.getProjectItemID().getItemID()))
        {
          hasAtLeastOneEdit = true;
          return stopVisit;
        }

        return continueVisit;
      });

  if (hasAtLeastOneEdit) return;

  this->add();
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
  ext::visit<VisitDepth::shallow>(
      edit,
      [](JuceTrack& track) {
        if (auto audioTrack = ext::isAudioTrack(track))
          ext::init(*audioTrack);
      });
}


void EditManager::updateBpm() const
{
  this->masterBpmSetting->setBpm(this->bpm);
  this->masterTransport->setLoopPoint2(
      this->masterTempo->beatsToTime(loopEndBeat));

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


  auto& transport = edit->getTransport();
  transport.ensureContextAllocated();

  auto& tempo = edit->tempoSequence;
  tempo.createEmptyState();

  auto bpmSetting = tempo.insertTempo(tempoStartTime);
  bpmSetting->setBpm(this->bpm);

  auto timeSigSetting = tempo.insertTimeSig(tempoStartTime);
  timeSigSetting->numerator = timeSigNumerator;
  timeSigSetting->denominator = timeSigDenominator;

  transport.looping = true;
  transport.setLoopPoint1(tempo.beatsToTime(loopStartBeat));
  transport.setLoopPoint2(tempo.beatsToTime(loopEndBeat));

  ext::setPositionBeats(transport, loopStartBeat);


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
