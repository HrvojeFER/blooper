#ifndef BLOOPER_CORE_WINDOW_HPP
#define BLOOPER_CORE_WINDOW_HPP


#include <blooper/fwd.hpp>


BLOOPER_NAMESPACE_BEGIN

class CoreWindow : public juce::DocumentWindow
{
public:
    CoreWindow(const juce::String& name, CoreContext& context);
    ~CoreWindow() override;


protected:
    CoreContext& context;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoreWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CORE_WINDOW_HPP
