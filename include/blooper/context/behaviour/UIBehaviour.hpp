#ifndef BLOOPER_UI_BEHAVIOUR_HPP
#define BLOOPER_UI_BEHAVIOUR_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;


class UIBehaviour : public te::UIBehaviour
{
public:
    UIBehaviour(CoreContext& context);
    ~UIBehaviour() override;


    std::unique_ptr<juce::Component> createPluginWindow(
            te::PluginWindowState& windowState) override;

    void recreatePluginWindowContentAsync(
            te::Plugin& plugin) override;


    void showProjectScreen() override;

    void showSettingsScreen() override;


private:
    CoreContext& context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIBehaviour);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_UI_BEHAVIOUR_HPP
