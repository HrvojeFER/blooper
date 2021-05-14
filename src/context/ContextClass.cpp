#include <blooper/context/ContextClass.hpp>


BLOOPER_NAMESPACE_BEGIN

Context::Context()
    : engine(),
      edit(),
      project()
{
    auto propertyStorage = new blooper::PropertyStorage();
    auto uiBehaviour = new blooper::UIBehaviour();
    auto engineBehaviour = new blooper::EngineBehaviour();

    auto newEngine = new te::Engine(
            std::unique_ptr<te::PropertyStorage>(propertyStorage),
            std::unique_ptr<te::UIBehaviour>(uiBehaviour),
            std::unique_ptr<te::EngineBehaviour>(engineBehaviour));

    propertyStorage->engine = newEngine;
    uiBehaviour->engine = newEngine;
    engineBehaviour->engine = newEngine;

    engine = std::unique_ptr<te::Engine>(newEngine);





    transport = &getEdit().getTransport();
}

Context::~Context() = default;

BLOOPER_NAMESPACE_END
