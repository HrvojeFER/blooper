#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginEditorWindow::PluginEditorWindow(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : CoreWindowBase(
          "",
          context,
          std::move(state)),
      options(std::move(options)),

      plugin(std::move(plugin)),

      component()
{
  // TODO: better

  PluginEditorComponent::Options componentOptions{};

  this->component =
      std::make_unique<PluginEditorComponent>(
          getContext(),
          getState().getOrCreateChildWithName(
              PluginEditorComponent::stateId,
              nullptr),
          getPluginRef(),
          std::move(componentOptions));

  setContentNonOwned(
      &getComponent(),
      true);


  this->initialiseResizingLimits();
  this->initialiseResizingBehaviour();
  this->initialiseVisibilityBehaviour();
}

[[maybe_unused]] void PluginEditorWindow::recreateContent()
{
  // TODO: better

  JUCE_AUTORELEASEPOOL
  {
    this->getComponent().getContent().recreate();
    this->initialiseResizingBehaviour();
  }
}

[[maybe_unused]] void PluginEditorWindow::recreateContentAsync()
{
  // TODO: better

  util::callAsync(
      [safePointer =
           SafePointer<PluginEditorWindow>(
               this)]() mutable {
        if (safePointer) safePointer->recreateContent();
      });
}

[[maybe_unused]] void PluginEditorWindow::initialiseResizingLimits()
{
  // TODO: better

  getConstrainer()->setMinimumOnscreenAmounts(
      0x10000,
      50,
      30,
      50);

  auto bounds =
      this->getLocalBounds() +
      this->getPlugin().windowState->lastWindowBounds.getPosition();

  setBounds(bounds);

  setResizeLimits(
      100,
      50,
      4000,
      4000);

  setBoundsConstrained(bounds);
}

[[maybe_unused]] void PluginEditorWindow::initialiseResizingBehaviour()
{
  // TODO: better

  auto& content = getComponent().getContent();
  auto  isResizeable = content.checkIsResizeable();

  setResizable(isResizeable, false);
  if (isResizeable) setConstrainer(content.getConstrainer());
}

[[maybe_unused]] void PluginEditorWindow::initialiseVisibilityBehaviour()
{
  // TODO: better

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "Simplify"
#endif

  if constexpr (shouldAddPluginWindowToDesktop)

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif

  {
    setAlwaysOnTop(true);
    addToDesktop();
  }
}

void PluginEditorWindow::moved()
{
  getPlugin().windowState->lastWindowBounds = getBounds();
  getPlugin().edit.pluginChanged(getPlugin());
}

void PluginEditorWindow::userTriedToCloseWindow()
{
  getPlugin().windowState->closeWindowExplicitly();
}

void PluginEditorWindow::closeButtonPressed()
{
  userTriedToCloseWindow();
}

[[nodiscard]] float PluginEditorWindow::getDesktopScaleFactor() const
{
  return 1.0f;
}


[[maybe_unused]] std::unique_ptr<PluginEditorWindow> showPluginEditorWindow(
    AbstractCoreContext&        context,
    JucePluginRef               plugin,
    PluginEditorWindow::Options options)
{
  if (!plugin) return nullptr;

  auto messageLoopBlocker =
      util::blockMessageLoopInScopeIfNeeded(context, *plugin);
  auto dpiDisabler =
      util::disableDPIInScopeIfNeeded(context, *plugin);

  auto window = std::make_unique<PluginEditorWindow>(
      context,
      context.getState().getOrCreateChildWithName(
          PluginEditorWindow::stateId,
          nullptr),
      std::move(plugin),
      std::move(options));

  window->show();

  return window;
}

BLOOPER_NAMESPACE_END
