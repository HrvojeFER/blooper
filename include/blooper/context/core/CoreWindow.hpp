#ifndef BLOOPER_CORE_WINDOW_HPP
#define BLOOPER_CORE_WINDOW_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;

class CoreWindow : public juce::DocumentWindow
{
public:
    CoreWindow(CoreContext& context);
    ~CoreWindow() override;

private:
    CoreContext& context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoreWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CORE_WINDOW_HPP
