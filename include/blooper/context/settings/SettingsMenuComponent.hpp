#ifndef BLOOPER_SETTINGS_MENU_COMPONENT_HPP
#define BLOOPER_SETTINGS_MENU_COMPONENT_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;


class SettingsMenuComponent : public juce::Component
{
public:
    explicit SettingsMenuComponent(CoreContext& context);
    ~SettingsMenuComponent() override;


private:
    CoreContext& engine;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_SETTINGS_MENU_COMPONENT_HPP
