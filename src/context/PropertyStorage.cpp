#include <blooper/context/PropertyStorage.hpp>


BLOOPER_NAMESPACE_BEGIN

PropertyStorage::PropertyStorage()
    : te::PropertyStorage(JUCE_APPLICATION_NAME_STRING),
      engine(nullptr)
{ }

PropertyStorage::~PropertyStorage() = default;

BLOOPER_NAMESPACE_END
