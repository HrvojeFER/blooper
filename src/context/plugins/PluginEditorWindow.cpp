#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginEditorWindow::PluginEditorWindow(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : CoreWindowBase(
          plugin->getName(),
          context,
          move(state)),
      options(move(options)),

      plugin(move(plugin))
{
  this->component =
      std::make_unique<PluginEditorComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              PluginEditorComponent::stateId,
              nullptr),
          this->getPluginRef(),
          PluginEditorComponent::Options{});

  this->setContentNonOwned(
      this->component.get(),
      true);


  this->initialiseResizingLimits();
  this->initialiseResizingBehaviour();
  this->initialiseVisibilityBehaviour();
}

PluginEditorWindow::~PluginEditorWindow()
{
  this->getPluginEdit().flushPluginStateIfNeeded(this->getPlugin());
}


[[maybe_unused]] void PluginEditorWindow::recreateContent()
{
  JUCE_AUTORELEASEPOOL
  {
    this->getComponent().recreate();
    this->initialiseResizingBehaviour();
  }
}

[[maybe_unused]] void PluginEditorWindow::initialiseResizingLimits()
{
  auto bounds = this->getLocalBounds() + this->getLastBounds().getPosition();

  auto halfHeight = bounds.getHeight() / 2;
  auto halfWidth = bounds.getWidth() / 2;

  this->getConstrainer()
      ->setMinimumOnscreenAmounts(
          halfHeight,
          halfWidth,
          halfHeight,
          halfWidth);

  this->setResizeLimits(
      PluginEditorWindow::minimumWidth,
      PluginEditorWindow::minimumHeight,
      PluginEditorWindow::maximumWidth,
      PluginEditorWindow::maximumHeight);

  this->setBoundsConstrained(bounds);
}

[[maybe_unused]] void PluginEditorWindow::initialiseResizingBehaviour()
{
  this->setResizable(
      this->getComponent().checkIsResizable(),
      false);

  this->setConstrainer(
      this->getComponent().getConstrainer());
}

[[maybe_unused]] void PluginEditorWindow::initialiseVisibilityBehaviour()
{
#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "Simplify"
#endif

  if constexpr (shouldAddPluginWindowToDesktop)

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif

  {
    this->setAlwaysOnTop(true);
    this->addToDesktop();
  }
}


// Window

[[maybe_unused]] void PluginEditorWindow::moved()
{
  this->getLastBounds() = this->getBounds();
  this->getPluginEdit().pluginChanged(this->getPlugin());
}

[[maybe_unused]] void PluginEditorWindow::userTriedToCloseWindow()
{
  this->getWindowState().closeWindowExplicitly();
}

[[maybe_unused]] void PluginEditorWindow::closeButtonPressed()
{
  this->userTriedToCloseWindow();
}

[[maybe_unused, nodiscard]] float
PluginEditorWindow::getDesktopScaleFactor() const
{
  return 1.0f;
}


[[maybe_unused]] PluginEditorWindow* showPluginEditorWindow(
    AbstractCoreContext&        context,
    JucePluginRef               plugin,
    PluginEditorWindow::Options options)
{
  if (!plugin) return nullptr;


  PluginEditorWindow* window;

  {
    auto messageLoopBlocker =
        util::blockMessageLoopInScopeIfNeeded(context, *plugin);
    auto dpiDisabler =
        util::disableDPIInScopeIfNeeded(context, *plugin);

    window = new PluginEditorWindow(
        context,
        context.getState().getOrCreateChildWithName(
            PluginEditorWindow::stateId,
            nullptr),
        move(plugin),
        move(options));
  }

  window->show();


  return window;
}

BLOOPER_NAMESPACE_END
