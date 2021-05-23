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


  std::unique_ptr<juce::Component> createPluginWindow(
      te::PluginWindowState& windowState) override;

  void recreatePluginWindowContentAsync(
      te::Plugin& plugin) override;


  void showProjectScreen() override;

  void showSettingsScreen() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIBehaviour);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_UI_BEHAVIOUR_HPP
