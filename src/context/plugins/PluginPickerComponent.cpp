#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginPickerComponent::PluginPickerComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      pluginTree(),

      popup()
{
}


#ifdef __JETBRAINS_IDE__ // recursive calls chains
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "misc-no-recursion"
#endif

class PluginPickerComponent::Popup : public juce::PopupMenu
{
 public:
  explicit Popup(PluginTreeBase* pluginTree);
};

PluginPickerComponent::Popup::Popup(PluginTreeBase* pluginTree)
{
  visit(
      pluginTree,
      [this](auto& subGroup) {
        this->addSubMenu(
            subGroup.getName(),
            PluginPickerComponent::Popup(&subGroup),
            true);
      },
      [this](auto& subItem) {
        this->addItem(
            getId(subItem),
            subItem.getName(),
            true,
            false);
      });
}

JucePluginRef PluginPickerComponent::runPopup()
{
  if (!pluginTree)
    pluginTree = std::make_unique<PluginTreeGroup>(getContext());

  if (!popup)
    popup = std::make_unique<Popup>(pluginTree.get());

  auto idOfUserSelection = popup->show();
  if (!idOfUserSelection) return nullptr;

  auto item = findIn(pluginTree.get(), idOfUserSelection);
  if (!item) return nullptr;

  return item->createPlugin();
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif


int PluginPickerComponent::getId(const PluginTreeBase& item)
{
  return item.getName().hashCode();
}

class PluginTreeItem* PluginPickerComponent::findIn(
    PluginTreeBase* pluginTree,
    int             id)
{
  PluginTreeItem* result = nullptr;

  visit<PluginTreeVisitType::deep>(
      pluginTree,
      [](auto&) {},
      [&result, id](auto& subItem) {
        if (getId(subItem) == id) result = &subItem;
      });

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
