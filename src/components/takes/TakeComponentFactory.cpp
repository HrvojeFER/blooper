#include <blooper/components/takes/TakeComponentFactory.hpp>

#include <blooper/internal/abstract/matchers.hpp>

#include <blooper/components/takes/WaveAudioTakeComponent.hpp>
#include <blooper/components/takes/MidiTakeComponent.hpp>
#include <blooper/components/takes/RecordingTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

static inline constexpr auto makeTakeComponentFactory =
    ([](auto& context, auto&& state, auto&& options) {
      return [&context,
              state = BLOOPER_FORWARD(state),
              options = BLOOPER_FORWARD(options)](
                 auto componentType, auto optionsType) {
        return [&context,
                state = state,
                options = options](auto takePtr) mutable
               -> std::unique_ptr<AbstractTakeContentComponent> {
          return std::make_unique<decltype(meta::source(componentType))>(
              context,
              move(state),
              *takePtr,
              decltype(meta::source(optionsType)){
                  options.shouldResizeItself,
                  options.shouldResizeParentWhenResized,
                  options.timePixelConverter});
        };
      };
    });


[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef& take,
    TakeComponentOptions   options)
{
  return createTakeComponent(
      context,
      move(state),
      std::addressof(take),
      move(options));
}

[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef* take,
    TakeComponentOptions   options)
{
  if (!take) return nullptr;
  if (take->isInvalid()) return nullptr;

  auto makeComponentConstructor =
      makeTakeComponentFactory(
          context,
          move(state),
          move(options));

  auto matchTake =
      matcherFold(
          makeDynamicMatcher(
              meta::type_c<const WaveAudioTakeRef*>,
              makeComponentConstructor(
                  meta::type_c<WaveAudioTakeComponent>,
                  meta::type_c<WaveAudioTakeComponentOptions>)),
          makeDynamicMatcher(
              meta::type_c<const MidiTakeRef*>,
              makeComponentConstructor(
                  meta::type_c<MidiTakeComponent>,
                  meta::type_c<MidiTakeComponentOptions>)));

  // TODO: not implemented
  return matchTake(take).value_or(nullptr);
}


[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createRecordingTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef& take,
    TakeComponentOptions   options)
{
  return createRecordingTakeComponent(
      context,
      move(state),
      std::addressof(take),
      move(options));
}

[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createRecordingTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef* take,
    TakeComponentOptions   options)
{
  if (!take) return nullptr;
  if (take->isInvalid()) return nullptr;

  auto makeComponentConstructor =
      makeTakeComponentFactory(
          context,
          move(state),
          move(options));

  auto matchTake =
      matcherFold(
          makeDynamicMatcher(
              meta::type_c<const WaveAudioTakeRef*>,
              makeComponentConstructor(
                  meta::type_c<RecordingTakeComponent>,
                  meta::type_c<RecordingTakeComponentOptions>)));

  // TODO: not implemented
  return matchTake(take).value_or(nullptr);
}

BLOOPER_NAMESPACE_END
