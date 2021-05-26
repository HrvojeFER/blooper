#ifndef BLOOPER_PLUGIN_CONTENT_FACTORY_HPP
#define BLOOPER_PLUGIN_CONTENT_FACTORY_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class AbstractPluginContentComponent;

struct PluginContentOptions
{
};

[[maybe_unused, nodiscard]] std::unique_ptr<AbstractPluginContentComponent>
createPluginContent(
    AbstractCoreContext& context,
    JucePluginRef        plugin,
    PluginContentOptions options = {});


// abstract and base

class AbstractPluginContentComponent :
    public virtual AbstractCoreComponent
{
 public:
  [[maybe_unused]] inline AbstractPluginContentComponent() = default;

  [[maybe_unused]] inline ~AbstractPluginContentComponent() override = default;


  [[maybe_unused, nodiscard]] inline virtual const JucePlugin&
  getPlugin() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual JucePluginConstRef
  getPluginRef() const noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual const JuceConstrainer*
  getConstrainer() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual JuceConstrainer*
  getConstrainer() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual bool
  checkIsResizeable() const noexcept = 0;


  [[maybe_unused]] virtual void recreate() = 0;

  [[maybe_unused]] virtual void recreateAsync() = 0;


 protected:
  [[maybe_unused, nodiscard]] inline virtual JucePlugin&
  getPlugin() noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual JucePluginRef
  getPluginRef() noexcept = 0;
};

template<typename TPluginRef>
class PluginContentComponentBase :
    public virtual AbstractPluginContentComponent,
    public CoreComponentBase
{
  static_assert(
      isJucePluginRef(meta::type_c<TPluginRef>),
      "PluginContentComponentBase requires a JucePluginRef.");


 public:
  [[maybe_unused]] explicit PluginContentComponentBase(
      AbstractCoreContext& context,
      State                state,
      TPluginRef           plugin);


  [[maybe_unused, nodiscard]] inline const JucePlugin&
  getPlugin() const noexcept final;

  [[maybe_unused, nodiscard]] inline JucePluginConstRef
  getPluginRef() const noexcept final;


 protected:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept final;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept final;


  TPluginRef plugin;
};


// Implementations

class NotImplementedPluginContentComponent final :
    public PluginContentComponentBase<JucePluginRef>
{
 public:
  BLOOPER_STATE_ID(NotImplementedPluginContentComponent);


  struct Options
  {
  } options;

  [[maybe_unused]] explicit NotImplementedPluginContentComponent(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options);


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept final;


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizeable() const noexcept final;


  void resized() override;


  [[maybe_unused]] void recreate() override;

  [[maybe_unused]] void recreateAsync() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      NotImplementedPluginContentComponent)
};

struct ExternalPluginContentComponent final :
    public PluginContentComponentBase<JuceExternalPluginRef>
{
 public:
  BLOOPER_STATE_ID(ExternalPluginContentComponent);

  inline constexpr static auto minHeight = 8;
  inline constexpr static auto minWidth = 8;


  struct Options
  {
  } options;

  [[maybe_unused]] explicit ExternalPluginContentComponent(
      AbstractCoreContext&  context,
      State                 state,
      JuceExternalPluginRef plugin,
      Options               options = {});


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept final;


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizeable() const noexcept final;


  void resized() override;

  void childBoundsChanged(JuceComponent* component) override;


  [[maybe_unused]] void recreate() override;

  [[maybe_unused]] void recreateAsync() override;


 private:
  std::unique_ptr<JucePluginContent> content;


  void resizeToFitEditor(bool force = false);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExternalPluginContentComponent)
};


// TODO: internal plugins here


// Inline implementations

template<typename TPluginRef>
[[maybe_unused]] PluginContentComponentBase<TPluginRef>::
    PluginContentComponentBase(
        AbstractCoreContext& context,
        State                state,
        TPluginRef           plugin)
    : CoreComponentBase(
          context,
          std::move(state)),
      plugin(std::move(plugin))
{
}

template<typename TPluginRef>
[[maybe_unused]] const JucePlugin&
PluginContentComponentBase<TPluginRef>::getPlugin() const noexcept
{
  return *plugin;
}

template<typename TPluginRef>
[[maybe_unused]] JucePlugin&
PluginContentComponentBase<TPluginRef>::getPlugin() noexcept
{
  return *plugin;
}

template<typename TPluginRef>
[[maybe_unused]] JucePluginConstRef
PluginContentComponentBase<TPluginRef>::getPluginRef() const noexcept
{
  return plugin;
}

template<typename TPluginRef>
[[maybe_unused]] JucePluginRef
PluginContentComponentBase<TPluginRef>::getPluginRef() noexcept
{
  return plugin;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_CONTENT_FACTORY_HPP
