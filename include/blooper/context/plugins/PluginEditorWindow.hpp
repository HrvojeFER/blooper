#ifndef BLOOPER_PLUGIN_EDITOR_WINDOW_HPP
#define BLOOPER_PLUGIN_EDITOR_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

#if JUCE_LINUX
constexpr bool shouldAddPluginWindowToDesktop = false;
#else
constexpr bool shouldAddPluginWindowToDesktop = true;
#endif

class PluginEditorWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(PluginEditorWindow);

  // closest to 100 width
  [[maybe_unused]] inline constexpr static auto minimumWidth = 128;
  [[maybe_unused]] inline constexpr static auto minimumHeight = 72;

  // 8K
  [[maybe_unused]] inline constexpr static auto maximumWidth = 7680;
  [[maybe_unused]] inline constexpr static auto maximumHeight = 4320;


  struct Options
  {
  } options;

  explicit PluginEditorWindow(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options = {});

  ~PluginEditorWindow() override;


  [[maybe_unused, nodiscard]] inline const JucePlugin&
  getPlugin() const noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginConstRef
  getPluginRef() const noexcept;


  [[maybe_unused]] inline void show();


  [[maybe_unused]] void recreateContent();


 private:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept;


  [[maybe_unused, nodiscard]] inline const JucePluginWindowState&
  getWindowState() const noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginWindowState&
  getWindowState() noexcept;


  [[maybe_unused, nodiscard]] inline const JuceBounds&
  getLastBounds() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceBounds&
  getLastBounds() noexcept;


  [[maybe_unused, nodiscard]] inline const JuceEdit&
  getPluginEdit() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceEdit&
  getPluginEdit() noexcept;


  [[maybe_unused, nodiscard]] inline const AbstractPluginContentComponent&
  getComponent() const noexcept;

  [[maybe_unused, nodiscard]] inline AbstractPluginContentComponent&
  getComponent() noexcept;


  JucePluginRef plugin;

  std::unique_ptr<AbstractPluginContentComponent> component;


  [[maybe_unused]] void initialiseResizingLimits();

  [[maybe_unused]] void initialiseResizingBehaviour();

  [[maybe_unused]] void initialiseVisibilityBehaviour();


  // Window

 private:
  [[maybe_unused]] void moved() override;

  [[maybe_unused]] void closeButtonPressed() override;

  [[maybe_unused]] void userTriedToCloseWindow() override;

  [[maybe_unused, nodiscard]] float getDesktopScaleFactor() const override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditorWindow)
};


[[maybe_unused]] PluginEditorWindow* showPluginEditorWindow(
    AbstractCoreContext&        context,
    JucePluginRef               plugin,
    PluginEditorWindow::Options options = {});


[[maybe_unused]] const JucePlugin&
PluginEditorWindow::getPlugin() const noexcept
{
  return *this->plugin;
}

[[maybe_unused]] JucePlugin&
PluginEditorWindow::getPlugin() noexcept
{
  return *this->plugin;
}

[[maybe_unused]] JucePluginConstRef
PluginEditorWindow::getPluginRef() const noexcept
{
  return this->plugin;
}

[[maybe_unused]] JucePluginRef
PluginEditorWindow::getPluginRef() noexcept
{
  return this->plugin;
}


[[maybe_unused]] const JucePluginWindowState&
PluginEditorWindow::getWindowState() const noexcept
{
  return *this->plugin->windowState;
}

[[maybe_unused]] JucePluginWindowState&
PluginEditorWindow::getWindowState() noexcept
{
  return *this->plugin->windowState;
}

[[maybe_unused]] const JuceBounds&
PluginEditorWindow::getLastBounds() const noexcept
{
  return this->plugin->windowState->lastWindowBounds;
}

[[maybe_unused]] JuceBounds&
PluginEditorWindow::getLastBounds() noexcept
{
  return this->plugin->windowState->lastWindowBounds;
}

[[maybe_unused]] const JuceEdit&
PluginEditorWindow::getPluginEdit() const noexcept
{
  return this->plugin->edit;
}

[[maybe_unused]] JuceEdit&
PluginEditorWindow::getPluginEdit() noexcept
{
  return this->plugin->edit;
}

[[maybe_unused]] const AbstractPluginContentComponent&
PluginEditorWindow::getComponent() const noexcept
{
  return *this->component;
}

[[maybe_unused]] AbstractPluginContentComponent&
PluginEditorWindow::getComponent() noexcept
{
  return *this->component;
}


[[maybe_unused]] void PluginEditorWindow::show()
{
  this->setBoundsConstrained(this->getBounds());
  this->toFront(false);

  this->setVisible(true);
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_EDITOR_WINDOW_HPP
