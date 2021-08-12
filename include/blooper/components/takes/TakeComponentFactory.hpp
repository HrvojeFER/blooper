#ifndef BLOOPER_TAKE_COMPONENT_FACTORY_HPP
#define BLOOPER_TAKE_COMPONENT_FACTORY_HPP

#include <blooper/internal/abstract/takes.hpp>

#include <blooper/components/takes/TakeComponentOptions.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef& take,
    TakeComponentOptions   options = {});

[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createTakeComponent(
    AbstractCoreContext&   context,
    State                  state,
    const AbstractTakeRef* take,
    TakeComponentOptions   options = {});


[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createRecordingTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef& take,
    TakeComponentOptions   options);

[[maybe_unused]] std::unique_ptr<AbstractTakeContentComponent>
createRecordingTakeComponent(
    AbstractContext&       context,
    State                  state,
    const AbstractTakeRef* take,
    TakeComponentOptions   options);

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TAKE_COMPONENT_FACTORY_HPP
