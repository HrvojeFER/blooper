#ifndef BLOOPER_PROJECT_EXT_HPP
#define BLOOPER_PROJECT_EXT_HPP


BLOOPER_NAMESPACE_BEGIN

static inline bool hasEdit(te::Project& project)
{
    for (int i = 0; i < project.getNumProjectItems(); ++i)
        if (project.getProjectItemAt(i)->isEdit())
            return true;

    return false;
}

static inline std::unique_ptr<te::Edit> getEdit(
        te::Project& project,
        te::Engine&  engine)
{
    for (int i = 0; i < project.getNumProjectItems(); ++i)
    {
        auto item = project.getProjectItemAt(i);
        if (item->isEdit())
            return te::loadEditFromFile(
                    engine,
                    item->getSourceFile());
    }

    return nullptr;
}

static inline std::unique_ptr<te::Edit> ensureEdit(
        te::Project& project,
        te::Engine&  engine)
{
    if (auto edit = getEdit(project, engine))
        return edit;

    return te::loadEditFromFile(
            engine,
            project.createNewEdit()->getSourceFile());
}

BLOOPER_NAMESPACE_END


#endif //BLOOPER_PROJECT_EXT_HPP
