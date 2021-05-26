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
        std::move(pluginContentState),
        externalPlugin,
        std::move(externalPluginOptions));
  }


  NotImplementedPluginContentComponent::Options notImplementedOptions{};

  return std::make_unique<NotImplementedPluginContentComponent>(
      context,
      std::move(pluginContentState),
      std::move(plugin),
      std::move(notImplementedOptions));
}


NotImplementedPluginContentComponent::NotImplementedPluginContentComponent(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : PluginContentComponentBase(
          context,
          std::move(state),
          std::move(plugin)),
      options(std::move(options))
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
NotImplementedPluginContentComponent::checkIsResizeable() const noexcept
{
  return true;
}

void NotImplementedPluginContentComponent::resized()
{
}

[[maybe_unused]] void NotImplementedPluginContentComponent::recreate()
{
}

[[maybe_unused]] void NotImplementedPluginContentComponent::recreateAsync()
{
}


ExternalPluginContentComponent::ExternalPluginContentComponent(
    AbstractCoreContext&  context,
    State                 state,
    JuceExternalPluginRef plugin,
    Options               options)
    : PluginContentComponentBase(
          context,
          std::move(state),
          std::move(plugin)),
      options(std::move(options)),

      content()
{
  this->recreate();
}

[[maybe_unused]] const JuceConstrainer*
ExternalPluginContentComponent::getConstrainer() const noexcept
{
  if (!content || checkIsResizeable()) return nullptr;

  return content->getConstrainer();
}

[[maybe_unused]] JuceConstrainer*
ExternalPluginContentComponent::getConstrainer() noexcept
{
  if (!content || checkIsResizeable()) return nullptr;

  return content->getConstrainer();
}

[[maybe_unused]] bool
ExternalPluginContentComponent::checkIsResizeable() const noexcept
{
  return false;
}

void ExternalPluginContentComponent::resized()
{
  auto availableArea = getLocalBounds();

  if (content) content->setBounds(availableArea);
}

void ExternalPluginContentComponent::childBoundsChanged(
    JuceComponent* component)
{
  if (component == content.get())
  {
    getPlugin().edit.pluginChanged(getPlugin());
    resizeToFitEditor();
  }
}

[[maybe_unused]] void ExternalPluginContentComponent::recreate()
{
  this->content.reset();

  JUCE_AUTORELEASEPOOL
  {
    if (auto instance = this->plugin->getAudioPluginInstance())
    {
      this->content.reset(instance->createEditorIfNeeded());

      if (!this->content)
        this->content =
            std::make_unique<JuceGenericPluginContent>(*instance);

      addAndMakeVisible(*content);
    }
  }

  resizeToFitEditor(true);
}

[[maybe_unused]] void ExternalPluginContentComponent::recreateAsync()
{
  util::callAsync(
      [safePointer =
           SafePointer<ExternalPluginContentComponent>(
               this)]() mutable {
        if (safePointer) safePointer->recreate();
      });
}


void ExternalPluginContentComponent::resizeToFitEditor(bool force)
{
  if (force || !checkIsResizeable())
    setSize(
        juce::jmax(
            minWidth,
            content ? content->getWidth() : 0),
        juce::jmax(
            minHeight,
            content ? content->getHeight() : 0));
}


// TODO: Internal plugins here

BLOOPER_NAMESPACE_END
