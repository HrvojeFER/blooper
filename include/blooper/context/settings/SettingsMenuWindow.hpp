#ifndef BLOOPER_SETTINGS_MENU_WINDOW_HPP
#define BLOOPER_SETTINGS_MENU_WINDOW_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class SettingsMenuWindow : public CoreWindow
{
public:
    explicit SettingsMenuWindow(CoreContext& context);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_SETTINGS_MENU_WINDOW_HPP
