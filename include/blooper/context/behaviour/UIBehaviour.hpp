#ifndef BLOOPER_UI_BEHAVIOUR_HPP
#define BLOOPER_UI_BEHAVIOUR_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class UIBehaviour :
    public CoreContextualBase,
    public te::UIBehaviour
{
 public:
  explicit UIBehaviour(AbstractCoreContext& context);


  std::unique_ptr<JuceComponent> createPluginWindow(JucePluginWindowState& windowState) override;

  void recreatePluginWindowContentAsync(JucePlugin& plugin) override;


  void showProjectScreen() override;

  void showSettingsScreen() override;


 private:
  std::unique_ptr<juce::ApplicationCommandManager> commandManager;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIBehaviour);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_UI_BEHAVIOUR_HPP
