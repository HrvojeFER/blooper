#ifndef BLOOPER_PLUGIN_EDITOR_WINDOW_HPP
#define BLOOPER_PLUGIN_EDITOR_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginEditorWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(PluginEditorWindow);

  using componentType = class PluginEditorComponent;


  struct Options
  {
  } options;

  explicit PluginEditorWindow(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options = {});


  [[maybe_unused, nodiscard]] inline const JucePlugin&
  getPlugin() const noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginConstRef
  getPluginRef() const noexcept;


  [[maybe_unused]] inline void show();


  [[maybe_unused]] void recreateContent();

  [[maybe_unused]] void recreateContentAsync();


 private:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept;


  [[maybe_unused, nodiscard]] inline const componentType&
  getComponent() const noexcept;

  [[maybe_unused, nodiscard]] inline componentType&
  getComponent() noexcept;


  JucePluginRef plugin;

  std::unique_ptr<componentType> component;


  [[maybe_unused]] void initialiseResizingLimits();

  [[maybe_unused]] void initialiseResizingBehaviour();

  [[maybe_unused]] void initialiseVisibilityBehaviour();


  void moved() override;

  void userTriedToCloseWindow() override;

  void closeButtonPressed() override;

  [[nodiscard]] float getDesktopScaleFactor() const override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditorWindow)
};


[[maybe_unused]] std::unique_ptr<PluginEditorWindow> showPluginEditorWindow(
    AbstractCoreContext&        context,
    JucePluginRef               plugin,
    PluginEditorWindow::Options options = {});


[[maybe_unused]] const JucePlugin&
PluginEditorWindow::getPlugin() const noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePlugin&
PluginEditorWindow::getPlugin() noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePluginConstRef
PluginEditorWindow::getPluginRef() const noexcept
{
  return plugin;
}

[[maybe_unused]] JucePluginRef
PluginEditorWindow::getPluginRef() noexcept
{
  return plugin;
}

[[maybe_unused]] const PluginEditorWindow::componentType&
PluginEditorWindow::getComponent() const noexcept
{
  return *component;
}

[[maybe_unused]] PluginEditorWindow::componentType&
PluginEditorWindow::getComponent() noexcept
{
  return *component;
}

[[maybe_unused]] void
PluginEditorWindow::show()
{
  this->setVisible(true);
  this->toFront(false);
  this->setBoundsConstrained(this->getBounds());
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_EDITOR_WINDOW_HPP
