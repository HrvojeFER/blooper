#ifndef BLOOPER_SETTINGS_MENU_COMPONENT_HPP
#define BLOOPER_SETTINGS_MENU_COMPONENT_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN


class SettingsMenuComponent : public CoreComponent
{
public:
    explicit SettingsMenuComponent(CoreContext& context);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_SETTINGS_MENU_COMPONENT_HPP
