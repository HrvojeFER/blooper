#include <blooper/context/ContextClass.hpp>


BLOOPER_NAMESPACE_BEGIN

Context::~Context()
{
    if (project != nullptr)
    {
        project->save();
    }

    if (edit != nullptr)
    {
        te::EditFileOperations(*edit)
                .save(true,
                      true,
                      false);
    }
}

BLOOPER_NAMESPACE_END
