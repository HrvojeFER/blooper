#ifndef BLOOPER_BODY_WINDOW_HPP
#define BLOOPER_BODY_WINDOW_HPP


#include <blooper/style/LookAndFeel.hpp>

#include <blooper/body/BodyComponent.hpp>
#include <blooper/body/BodyComponentOld.hpp>


BLOOPER_NAMESPACE_BEGIN

class BodyWindow : public juce::DocumentWindow
{
public:
    explicit BodyWindow(Context& context);
    ~BodyWindow() override;

    std::function<void()> onClose;


    void closeButtonPressed() override;

private:
    Context& context;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyWindow)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_BODY_WINDOW_HPP
