#include <blooper/components/takes/MidiTakeComponent.hpp>

#include <blooper/internal/abstract/midi.hpp>
#include <blooper/internal/utils/gui.hpp>

BLOOPER_NAMESPACE_BEGIN

MidiTakeComponent::MidiTakeComponent(
    AbstractContext&         context,
    State                    state,
    MidiTakeRef              take,
    MidiTakeComponentOptions options)
    : MidiTakeComponent::base(
          context,
          move(state),
          move(take)),
      options(move(options))
{
  if (this->options.shouldResizeItself)
    this->getClip().state.addListener(this);
}

MidiTakeComponent::~MidiTakeComponent()
{
  this->getClip().state.removeListener(this);
}


// ValueTreeListener

void MidiTakeComponent::valueTreePropertyChanged(
    JuceState&                 state,
    const JuceStateIdentifier& id)
{
  if (state == this->getClip().state)
  {
    if (id == te::IDs::offset ||
        id == te::IDs::start ||
        id == te::IDs::length)
    {
      this->setBounds(
          util::withHorizontalRange(
              this->getBounds(),
              this->getTimePixelMapping(
                      dynamic_cast<AbstractTimePixelConverter*>(
                          this->getParentComponent()))
                  .pixels));

      if (this->options.shouldResizeParentWhenResized &&
          this->getParentComponent())
        this->getParentComponent()->resized();
    }
  }
}


// Component

void MidiTakeComponent::paint(juce::Graphics& g)
{
  base::paint(g);

  auto& midi = *this->getHeldTakeRef().midi;
  auto& clip = *this->getHeldTakeRef().clip;
  auto& edit = clip.edit;
  auto& tempo = edit.tempoSequence;

  auto mapping = this->getTimePixelMapping();
  auto noteHeight = this->getHeight() / midiNoteNumberCount;

  // TODO include other events
  for (auto note : midi.getNotes())
  {
    g.setColour(
        this->findColour(ColourId::white)
            .withAlpha(
                static_cast<float>(note->getVelocity() - minVelocity) /
                velocityValueCount));

    auto noteXPixels =
        mapping.convertToPixels(
            {tempo.beatsToTime(note->getRangeBeats().getStart()),
             tempo.beatsToTime(note->getRangeBeats().getEnd())});

    g.fillRect(
        static_cast<float>(noteXPixels.getStart()),
        static_cast<float>(
            this->getHeight() -
            (note->getNoteNumber() + 1) * noteHeight),
        static_cast<float>(noteXPixels.getLength()),
        static_cast<float>(noteHeight));
  }
}

BLOOPER_NAMESPACE_END
