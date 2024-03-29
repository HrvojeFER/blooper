#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] std::unique_ptr<JuceConstrainer>
createConstrainer(const AbstractPluginContentComponent& content) noexcept
{
  auto contentConstrainer = content.getConstrainer();

  if (contentConstrainer)
  {
    auto componentConstrainer = std::make_unique<JuceConstrainer>();

    util::copyConstrainer(
        *contentConstrainer,
        *componentConstrainer);

    return componentConstrainer;
  }
  else
  {
    return {};
  }
}


PluginEditorComponent::PluginEditorComponent(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : PluginContentComponentBase(
          move(plugin),
          context,
          move(state)),
      options(move(options)),

      content(
          createPluginContent(
              getContext(),
              getPluginRef(),
              PluginContentOptions{})),

      constrainer(createConstrainer(*this->content))
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
