#include <blooper/context/plugins/PluginPickerComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginPickerComponent::PluginPickerComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


class PluginPickerComponent::Popup : public juce::PopupMenu
{
 public:
  explicit Popup(PluginTreeBase& root);
};

PluginPickerComponent::Popup::Popup(PluginTreeBase& root)
{
  root.visit<VisitDepth::shallow>(
      meta::overload(
          [this](PluginTreeGroup& group) {
            this->addSubMenu(
                group.getName(),
                PluginPickerComponent::Popup(group),
                true);
          },
          [this](PluginTreeItem& item) {
            this->addItem(
                getId(item),
                item.getName(),
                true,
                false);
          }));
}

JucePluginRef PluginPickerComponent::runPopup()
{
  if (!this->pluginTree)
    this->pluginTree =
        std::make_unique<PluginTreeGroup>(
            this->getContext());

  if (!this->popup)
    this->popup =
        std::make_unique<Popup>(
            *this->pluginTree);

  auto idOfUserSelection = this->popup->show();
  if (!idOfUserSelection) return nullptr;

  auto item = PluginPickerComponent::findIn(
      *this->pluginTree,
      idOfUserSelection);
  if (!item) return nullptr;

  return item->createPlugin();
}


int PluginPickerComponent::getId(const PluginTreeBase& item)
{
  return item.getName().hashCode();
}

class PluginTreeItem* PluginPickerComponent::findIn(
    PluginTreeBase& root,
    int             id)
{
  PluginTreeItem* result = nullptr;

  root.visit<VisitDepth::deep>(
      meta::overload(
          [&result, id](PluginTreeItem& item) {
            if (getId(item) == id)
            {
              result = std::addressof(item);
              return false;
            }
            return true;
          }));

  return result;
}


[[maybe_unused]] JucePluginRef pickPlugin(
    AbstractContext&               context,
    PluginPickerComponent::Options options)
{
  return PluginPickerComponent(
             context,
             context.getState().getOrCreateChildWithName(
                 PluginPickerComponent::stateId,
                 nullptr),
             move(options))
      .runPopup();
}

BLOOPER_NAMESPACE_END
