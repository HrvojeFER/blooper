#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] std::unique_ptr<AbstractPluginContentComponent>
createPluginContent(
    AbstractCoreContext& context,
    JucePluginRef        plugin,
    PluginContentOptions)
{
  auto pluginContentState =
      context.getState().getOrCreateChildWithName(
          ext::getId(*plugin),
          nullptr);


  if (auto externalPlugin = dynamic_cast<te::ExternalPlugin*>(plugin.get()))
  {
    ExternalPluginContentComponent::Options externalPluginOptions{};

    return std::make_unique<ExternalPluginContentComponent>(
        context,
        move(pluginContentState),
        externalPlugin,
        move(externalPluginOptions));
  }


  // TODO: solution for builtin plugin contents


  NotImplementedPluginContentComponent::Options notImplementedOptions{};

  return std::make_unique<NotImplementedPluginContentComponent>(
      context,
      move(pluginContentState),
      move(plugin),
      move(notImplementedOptions));
}


NotImplementedPluginContentComponent::NotImplementedPluginContentComponent(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : PluginContentComponentBase(
          move(plugin),
          context,
          move(state)),
      options(move(options))
{
}

[[maybe_unused]] const JuceConstrainer*
NotImplementedPluginContentComponent::getConstrainer() const noexcept
{
  return nullptr;
}

[[maybe_unused]] JuceConstrainer*
NotImplementedPluginContentComponent::getConstrainer() noexcept
{
  return nullptr;
}

[[maybe_unused]] bool
NotImplementedPluginContentComponent::checkIsResizable() const noexcept
{
  return true;
}

void NotImplementedPluginContentComponent::resized()
{
}

[[maybe_unused]] void NotImplementedPluginContentComponent::recreate()
{
}


ExternalPluginContentComponent::ExternalPluginContentComponent(
    AbstractCoreContext&  context,
    State                 state,
    JuceExternalPluginRef plugin,
    Options               options)
    : ExternalPluginContentComponentBase(
          move(plugin),
          context,
          move(state)),
      options(move(options)),

      content()
{
  this->recreate();
}

[[maybe_unused]] const JuceConstrainer*
ExternalPluginContentComponent::getConstrainer() const noexcept
{
  if (!this->content || checkIsResizable()) return nullptr;

  return this->content->getConstrainer();
}

[[maybe_unused]] JuceConstrainer*
ExternalPluginContentComponent::getConstrainer() noexcept
{
  if (!this->content || checkIsResizable()) return nullptr;

  return this->content->getConstrainer();
}

[[maybe_unused]] bool
ExternalPluginContentComponent::checkIsResizable() const noexcept
{
  return false;
}

void ExternalPluginContentComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  if (this->content) this->content->setBounds(availableArea);
}

void ExternalPluginContentComponent::childBoundsChanged(
    JuceComponent* component)
{
  if (component == this->content.get())
  {
    this->getPlugin().edit.pluginChanged(this->getPlugin());
    this->resizeToFitContent();
  }
}

[[maybe_unused]] void ExternalPluginContentComponent::recreate()
{
  this->removeChildComponent(this->content.get());

  this->content.reset();

  JUCE_AUTORELEASEPOOL
  {
    if (auto instance = this->getHeldPlugin().getAudioPluginInstance())
    {
      this->content.reset(instance->createEditorIfNeeded());

      if (!this->content)
        this->content =
            std::make_unique<JuceGenericPluginContent>(
                *instance);

      addAndMakeVisible(*this->content);
    }
  }

  this->resizeToFitContent(true);
}


void ExternalPluginContentComponent::resizeToFitContent(bool force)
{
  if (force || !checkIsResizable())
    setSize(
        juce::jmax(
            ExternalPluginContentComponent::minWidth,
            this->content ? this->content->getWidth() : 0),
        juce::jmax(
            ExternalPluginContentComponent::minHeight,
            this->content ? this->content->getHeight() : 0));
}

BLOOPER_NAMESPACE_END
