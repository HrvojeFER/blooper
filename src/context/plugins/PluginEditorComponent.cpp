#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginEditorComponent::PluginEditorComponent(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : CoreComponentBase(
          context,
          std::move(state)),
      options(std::move(options)),

      plugin(std::move(plugin)),
      content(
          createPluginContent(
              getContext(),
              getPluginRef(),
              PluginContentOptions{})),

      constrainer(this->createConstrainer())
{
  ext::addAndMakeVisible(
      *this,
      *content);
}

void PluginEditorComponent::resized()
{
  auto availableArea = getLocalBounds();

  content->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
