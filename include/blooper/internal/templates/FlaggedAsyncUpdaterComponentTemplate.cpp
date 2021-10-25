#include <blooper/__path__>

BLOOPER_NAMESPACE_BEGIN

__name__Component::__name__Component(
    AbstractContext&         context,
    State                    state,
    __name__ComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

__name__Component::~__name__Component() = default;


// Component

void __name__Component::resized()
{
}


// ValueTreeListener

void __name__Component::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
}

void __name__Component::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
}

void __name__Component::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int              index)
{
}

void __name__Component::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
}


// FlaggedAsyncUpdater

void __name__Component::handleAsyncUpdate()
{
}

BLOOPER_NAMESPACE_END
