#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] PlayheadComponent::PlayheadComponent(
        te::Edit& e, EditViewState& evs)
    : edit(e), editViewState(evs)
{
    startTimerHz(30);
}

void PlayheadComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::yellow);
    g.drawRect(xPosition, 0, 2, getHeight());

    g.setColour(juce::Colours::red);
    g.drawRect(loopStart, 0, 2, getHeight());
    g.drawRect(loopEnd, 0, 2, getHeight());
}

bool PlayheadComponent::hitTest(int x, int)
{
    if (std::abs(x - xPosition) <= 3)
        return true;

    return false;
}

void PlayheadComponent::mouseDown(const juce::MouseEvent&)
{
    edit.getTransport().setUserDragging(true);
}

void PlayheadComponent::mouseUp(const juce::MouseEvent&)
{
    edit.getTransport().setUserDragging(false);
}

void PlayheadComponent::mouseDrag(const juce::MouseEvent& e)
{
    double t = editViewState.xToTime(e.x, getWidth());
    edit.getTransport().setCurrentPosition(t);
    timerCallback();
}

void PlayheadComponent::timerCallback()
{
    if (firstTimer)
    {
        // On Linux, don't set the mouse cursor until
        // after the Component has appeared
        firstTimer = false;
        setMouseCursor(juce::MouseCursor::LeftRightResizeCursor);
    }

    int newX = editViewState.timeToX(
            edit.getTransport().getCurrentPosition(),
            getWidth());

    auto newLoopRange = edit.getTransport().getLoopRange();
    auto newLoopStart = editViewState.timeToX(
            newLoopRange.getStart(),
            getWidth());
    auto newLoopEnd = editViewState.timeToX(
            newLoopRange.getEnd(),
            getWidth());

    if (newX != xPosition ||
        newLoopStart != loopStart ||
        newLoopEnd != loopEnd)
    {
        repaint();

        xPosition = newX;
        loopStart = newLoopStart;
        loopEnd = newLoopEnd;
    }
}

BLOOPER_NAMESPACE_END
