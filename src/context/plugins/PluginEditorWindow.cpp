#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginEditorWindow::PluginEditorWindow(
    AbstractCoreContext& context,
    State                state,
    JucePluginRef        plugin,
    Options              options)
    : CoreWindowBase(
          plugin->getOwnerTrack()->getName() +
              " " +
              plugin->getName(),
          context,
          move(state)),
      options(move(options)),

      plugin(move(plugin))
{
  PluginEditorComponent::Options componentOptions{};

  this->component =
      std::make_unique<PluginEditorComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              PluginEditorComponent::stateId,
              nullptr),
          this->getPluginRef(),
          move(componentOptions));

  this->component->setBounds(this->getBounds());

  this->setContentNonOwned(
      this->component.get(),
      true);


  this->initializeResizingBehaviour();
  this->initializeVisibilityBehaviour();
}

PluginEditorWindow::~PluginEditorWindow()
{
  this->finalizeVisibilityBehaviour();
  this->finalizeResizingBehaviour();

  this->getPluginEdit().flushPluginStateIfNeeded(this->getPlugin());
}


[[maybe_unused]] void PluginEditorWindow::recreateContent()
{
  this->getComponent().recreate();

  this->finalizeResizingBehaviour();
  this->initializeResizingBehaviour();
}


[[maybe_unused]] void PluginEditorWindow::initializeResizingBehaviour()
{
  this->setConstrainer(
      this->getComponent().getConstrainer());

  if (this->getConstrainer())
  {
    auto bounds =
        this->getLocalBounds() +
        this->getLastBounds().getPosition();

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

  this->setResizable(
      this->getComponent().checkIsResizable(),
      true);
}

[[maybe_unused]] void PluginEditorWindow::initializeVisibilityBehaviour()
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


[[maybe_unused]] void PluginEditorWindow::finalizeResizingBehaviour()
{
  this->setResizable(
      true,
      true);

  this->setConstrainer(
      nullptr);
}

[[maybe_unused]] void PluginEditorWindow::finalizeVisibilityBehaviour()
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
    this->removeFromDesktop();
    this->setAlwaysOnTop(false);
  }
}


// Window

[[maybe_unused]] void PluginEditorWindow::moved()
{
  this->getLastBounds() = this->getBounds();
  this->getPluginEdit().pluginChanged(this->getPlugin());
}

[[maybe_unused]] void PluginEditorWindow::closeButtonPressed()
{
  this->getWindowState().closeWindowExplicitly();
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
    [[maybe_unused]] const auto [messageLoopBlocker, DPIAwarenessDisabler] =
        util::declarePluginEditorCreationScope(
            context, *plugin);

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
