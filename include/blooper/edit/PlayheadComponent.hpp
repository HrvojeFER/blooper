#ifndef BLOOPER_PLAYHEAD_COMPONENT_HPP
#define BLOOPER_PLAYHEAD_COMPONENT_HPP


#include <blooper/internal/internal.hpp>


namespace blooper
{
using namespace juce;
namespace te = tracktion_engine;


class PlayheadComponent :
    public Component,
    private Timer
{
public:
    [[maybe_unused]] PlayheadComponent(te::Edit&, EditViewState&);

    void paint(Graphics& g) override;
    bool hitTest(int x, int y) override;
    void mouseDrag(const MouseEvent&) override;
    void mouseDown(const MouseEvent&) override;
    void mouseUp(const MouseEvent&) override;


private:
    void timerCallback() override;

    te::Edit&      edit;
    EditViewState& editViewState;

    int  xPosition = 0;
    bool firstTimer = true;
};
} // namespace blooper


#endif // BLOOPER_PLAYHEAD_COMPONENT_HPP
