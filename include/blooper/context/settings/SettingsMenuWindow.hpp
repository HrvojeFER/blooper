#ifndef BLOOPER_SETTINGS_MENU_WINDOW_HPP
#define BLOOPER_SETTINGS_MENU_WINDOW_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>

#include <blooper/context/settings/SettingsMenuComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

class SettingsMenuWindow : public juce::DocumentWindow
{
public:
    SettingsMenuWindow(te::Engine& engine);
    ~SettingsMenuWindow() override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_SETTINGS_MENU_WINDOW_HPP
