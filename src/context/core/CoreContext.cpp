#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

CoreContext::CoreContext()
    : engine(std::make_unique<te::Engine>(
              std::make_unique<PropertyStorage>(*this),
              std::make_unique<UIBehaviour>(*this),
              std::make_unique<EngineBehaviour>(*this))),

      lookAndFeel(std::make_unique<LookAndFeel>())
{
    engine->getProjectManager().loadList();
}

CoreContext::~CoreContext()
{
    if (engine != nullptr)
    {
        engine->getTemporaryFileManager()
                .getTempDirectory()
                .deleteRecursively();

        engine->getPropertyStorage().flushSettingsToDisk();
        engine->getProjectManager().saveList();
    }
}

BLOOPER_NAMESPACE_END
