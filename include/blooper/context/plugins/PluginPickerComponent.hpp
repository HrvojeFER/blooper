#ifndef BLOOPER_PLUGIN_PICKER_POPUP_HPP
#define BLOOPER_PLUGIN_PICKER_POPUP_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginPickerComponent : public ContextualBase
{
 public:
  explicit PluginPickerComponent(AbstractContext& context);


  te::Plugin::Ptr runPopup();


 private:
  std::unique_ptr<class PluginTreeBase> pluginTree;


  class Popup;
  std::unique_ptr<Popup> popup;

  static int getId(const class PluginTreeBase& item);

  static class PluginTreeItem* findIn(
      class PluginTreeBase* root,
      int                   hash);
};


[[maybe_unused, nodiscard]] te::Plugin::Ptr pickPlugin(
    AbstractContext& context);

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_PICKER_POPUP_HPP
