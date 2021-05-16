#include <blooper/context/behaviour/EngineBehaviour.hpp>

#include <blooper/context/core/core.hpp>

#include <blooper/context/devices/devices.hpp>
#include <blooper/context/settings/settings.hpp>
#include <blooper/context/plugins/plugins.hpp>
#include <blooper/context/projects/projects.hpp>


BLOOPER_NAMESPACE_BEGIN

EngineBehaviour::EngineBehaviour(CoreContext& context)
    : context(context)
{ }

EngineBehaviour::~EngineBehaviour() = default;

BLOOPER_NAMESPACE_END
