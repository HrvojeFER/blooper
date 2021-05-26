#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginComponent::PluginComponent(
    AbstractCoreContext& context,
    State                state,
    PluginRef            plugin,
    Options              options)
    : CoreComponentBase(
          context,
          std::move(state)),
      options(std::move(options)),

      plugin(std::move(plugin)),

      button(this)
{
  ext::addAndMakeVisible(
      *this,
      button);
}


void PluginComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  button.setBounds(availableArea);
}


PluginComponent::Button::Button(PluginComponent* parent)
    : parent(parent)
{
  setLookAndFeel(&parent->getLookAndFeel());

  this->setButtonText(
      parent->getPlugin().getName().substring(
          0,
          1));
}

void PluginComponent::Button::clicked(const juce::ModifierKeys& modifiers)
{
  if (modifiers.isPopupMenu())
  {
    juce::PopupMenu menu;

    menu.addItem(
        "Delete",
        [this] {
          this->parent->getPlugin().deleteFromParent();
        });

    menu.showAt(this);
  }
  else
  {
    parent->getPlugin().showWindowExplicitly();
  }
}

BLOOPER_NAMESPACE_END
