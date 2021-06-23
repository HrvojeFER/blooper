#ifndef BLOOPER_PLUGIN_PICKER_POPUP_HPP
#define BLOOPER_PLUGIN_PICKER_POPUP_HPP

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/PluginTree.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginPickerComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(PluginPickerComponent);


  struct Options
  {
  } options;

  explicit PluginPickerComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  JucePluginRef runPopup();


 private:
  std::unique_ptr<PluginTreeBase> pluginTree;


  class Popup;
  std::unique_ptr<Popup> popup;

  static int getId(const PluginTreeBase& item);

  static PluginTreeItem* findIn(
      PluginTreeBase* root,
      int             hash);
};


[[maybe_unused, nodiscard]] JucePluginRef pickPlugin(
    AbstractContext&               context,
    PluginPickerComponent::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_PICKER_POPUP_HPP
