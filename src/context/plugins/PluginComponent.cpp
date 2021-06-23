#include <blooper/context/plugins/PluginComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginComponent::PluginComponent(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options)),

      plugin(move(plugin)),

      button(this)
{
  ext::addAndMakeVisible(
      *this,
      this->button);
}


void PluginComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->button.setBounds(availableArea);
}


PluginComponent::Button::Button(PluginComponent* parent)
    : parent(parent)
{
  this->setLookAndFeel(&this->parent->getLookAndFeel());

  this->setButtonText(
      this->parent->getPlugin().getName().substring(
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
    this->parent->getPlugin().showWindowExplicitly();
  }
}

BLOOPER_NAMESPACE_END
