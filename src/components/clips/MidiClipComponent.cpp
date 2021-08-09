#include <blooper/components/clips/MidiClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MidiClipComponent::MidiClipComponent(
    AbstractContext&         context,
    State                    state,
    MidiClipComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

MidiClipComponent::~MidiClipComponent() = default;


void MidiClipComponent::paint (Graphics& g)
{
  ClipComponent::paint (g);

  if (auto mc = getMidiClip())
  {
    auto& seq = mc->getSequence();
    for (auto n : seq.getNotes())
    {
      double sBeat = mc->getStartBeat() + n->getStartBeat();
      double eBeat = mc->getStartBeat() + n->getEndBeat();

      auto s = editViewState.beatToTime (sBeat);
      auto e = editViewState.beatToTime (eBeat);

      if (auto p = getParentComponent())
      {
        double t1 = editViewState.timeToX (s, p->getWidth()) - getX();
        double t2 = editViewState.timeToX (e, p->getWidth()) - getX();

        double y = (1.0 - double (n->getNoteNumber()) / 127.0) * getHeight();

        g.setColour (Colours::white.withAlpha (n->getVelocity() / 127.0f));
        g.drawLine (float (t1), float (y), float (t2), float (y));
      }
    }
  }
}


// Component

void MidiClipComponent::resized()
{
}

BLOOPER_NAMESPACE_END
