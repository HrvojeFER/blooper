#ifndef BLOOPER_ABSTRACT_HPP
#define BLOOPER_ABSTRACT_HPP


BLOOPER_NAMESPACE_BEGIN

// ============================================================================
// Core context
// ============================================================================

[[maybe_unused]] inline constexpr auto isCoreContext =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(toCheck.getEngine(),
                                    toCheck.getLookAndFeel()) {});

class [[maybe_unused]] AbstractCoreContext
{
public:
    [[maybe_unused, nodiscard]] virtual inline const te::Engine&
    getEngine() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline te::Engine&
    getEngine() noexcept = 0;


    [[maybe_unused, nodiscard]] virtual inline const juce::LookAndFeel&
    getLookAndFeel() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline juce::LookAndFeel&
    getLookAndFeel() noexcept = 0;
};

static_assert(
        decltype(isCoreContext(std::declval<AbstractCoreContext>()))::value,
        "AbstractCoreContext must satisfy CoreContext.");


// Core component

[[maybe_unused]] inline constexpr auto isCoreComponent =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(static_cast<juce::Component&&>(toCheck)) {
                    static_assert(
                            decltype(isCoreContext(toCheck.getCoreContext()))::value,
                            "CoreComponent requires a CoreContext.");
                });


template<typename TCoreContext = AbstractCoreContext>
class [[maybe_unused]] AbstractCoreComponent : public juce::Component
{
    static_assert(
            decltype(isCoreContext(std::declval<TCoreContext>()))::value,
            "AbstractCoreComponent requires a CoreContext.");

public:
    [[maybe_unused, nodiscard]] virtual inline const TCoreContext&
    getCoreContext() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline TCoreContext&
    getCoreContext() noexcept = 0;
};

static_assert(
        decltype(isCoreComponent(std::declval<AbstractCoreComponent<>>()))::value,
        "AbstractCoreComponent must satisfy CoreComponent.");


// Core window

[[maybe_unused]] inline constexpr auto isCoreWindow =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(static_cast<juce::DocumentWindow&&>(toCheck)) {
                    static_assert(
                            decltype(isCoreContext(toCheck.getCoreContext()))::value,
                            "CoreWindow requires a CoreContext.");
                });
;

template<typename TCoreContext = AbstractCoreContext>
class [[maybe_unused]] AbstractCoreWindow : public juce::DocumentWindow
{
    static_assert(
            decltype(isCoreContext(std::declval<TCoreContext>()))::value,
            "AbstractCoreComponent requires a CoreContext.");

public:
    [[maybe_unused, nodiscard]] virtual inline const TCoreContext&
    getCoreContext() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline TCoreContext&
    getCoreContext() noexcept = 0;
};

static_assert(
        decltype(isCoreWindow(std::declval<AbstractCoreWindow<>>()))::value,
        "AbstractCoreWindow must satisfy CoreWindow.");


// ============================================================================
// Context
// ============================================================================

[[maybe_unused]] inline constexpr auto isContext =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(toCheck.getProject(),
                                    toCheck.getEdit(),
                                    toCheck.getTransport()) {
                    static_assert(
                            decltype(isCoreContext(toCheck))::value,
                            "Context must also be a CoreContext.");
                });
;

class [[maybe_unused]] AbstractContext : public AbstractCoreContext
{
public:
    [[maybe_unused, nodiscard]] virtual inline const te::Project&
    getProject() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline te::Project&
    getProject() noexcept = 0;


    [[maybe_unused, nodiscard]] virtual inline const te::Edit&
    getEdit() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline te::Edit&
    getEdit() noexcept = 0;


    [[maybe_unused, nodiscard]] virtual inline const te::TransportControl&
    getTransport() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline te::TransportControl&
    getTransport() noexcept = 0;
};

static_assert(
        decltype(isContext(std::declval<AbstractContext>()))::value,
        "AbstractContext must satisfy Context.");


//  component

[[maybe_unused]] inline constexpr auto isComponent =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(static_cast<juce::Component&&>(toCheck)) {
                    static_assert(
                            decltype(isContext(toCheck.getContext()))::value,
                            "Component requires a Context.");
                });


template<typename TContext = AbstractContext>
class [[maybe_unused]] AbstractComponent : public juce::Component
{
    static_assert(
            decltype(isContext(std::declval<TContext>()))::value,
            "AbstractComponent requires a Context.");

public:
    [[maybe_unused, nodiscard]] virtual inline const TContext&
    getContext() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline TContext&
    getContext() noexcept = 0;
};

static_assert(
        decltype(isComponent(std::declval<AbstractComponent<>>()))::value,
        "AbstractComponent must satisfy Component.");


//  window

[[maybe_unused]] inline constexpr auto isWindow =
        env::meta::is_valid(
                [](auto&& toCheck)
                        -> decltype(static_cast<juce::DocumentWindow&&>(toCheck)) {
                    static_assert(
                            decltype(isContext(toCheck.getContext()))::value,
                            "Window requires a Context.");
                });
;

template<typename TContext = AbstractContext>
class [[maybe_unused]] AbstractWindow : public juce::DocumentWindow
{
    static_assert(
            decltype(isContext(std::declval<TContext>()))::value,
            "AbstractWindow requires a Context.");

public:
    [[maybe_unused, nodiscard]] virtual inline const TContext&
    getContext() const noexcept = 0;

    [[maybe_unused, nodiscard]] virtual inline TContext&
    getContext() noexcept = 0;
};

static_assert(
        decltype(isWindow(std::declval<AbstractWindow<>>()))::value,
        "AbstractWindow must satisfy Window.");


BLOOPER_NAMESPACE_END


#endif // BLOOPER_ABSTRACT_HPP
