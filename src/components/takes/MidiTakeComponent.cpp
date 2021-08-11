#include <blooper/components/takes/MidiTakeComponent.hpp>

#include <blooper/internal/abstract/midi.hpp>
#include <blooper/internal/utils/style.hpp>

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
}

MidiTakeComponent::~MidiTakeComponent() = default;


// Component

void MidiTakeComponent::resized()
{
}

void MidiTakeComponent::paint(juce::Graphics& g)
{
  base::paint(g);

  if (!this->options.converter) return;
  auto& converter = *this->options.converter;

  if (this->getTakeRef().isInvalid()) return;
  auto& midi = *this->getTakeRef().midi;
  auto& clip = *this->getTakeRef().clip;
  auto& edit = clip.edit;
  auto& tempo = edit.tempoSequence;

  auto noteHeight = this->getHeight() / midiNoteNumberCount;


  auto timeToX = [this, &converter](double time) -> int {
    if (!this->getParentComponent()) return 0;
    auto& parent = *this->getParentComponent();

    return static_cast<int>(
               converter.convertToProgress(time)) *
               parent.getWidth() -
           this->getX();
  };

  for (auto note : midi.getNotes())
  {
    auto noteStartYInverted = (note->getNoteNumber() + 1) * noteHeight;
    auto noteStartY = this->getHeight() - (noteStartYInverted);

    auto noteStart = tempo.beatsToTime(note->getRangeBeats().getStart());
    auto noteStartX = timeToX(noteStart);
    auto noteEnd = tempo.beatsToTime(note->getRangeBeats().getEnd());
    auto noteEndX = timeToX(noteEnd);

    auto x = noteStartX;
    auto width = noteEndX - noteStartX;
    auto y = noteStartY;
    auto height = noteHeight;

    auto magnitude =
        static_cast<float>(note->getVelocity() - minVelocity) /
        velocityValueCount;

    g.setColour(
        this->findColour(ColourId::white)
            .withAlpha(magnitude));

    g.fillRect(
        static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(width),
        static_cast<float>(height));
  }
}

BLOOPER_NAMESPACE_END
