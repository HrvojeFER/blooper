#ifndef BLOOPER_PROJECT_WINDOW_HPP
#define BLOOPER_PROJECT_WINDOW_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectTreeBase
{
public:
    virtual ~ProjectTreeBase() = default;
    [[nodiscard]] virtual juce::String getUniqueName() const = 0;

    void addSubItem(ProjectTreeBase* itm)
    {
        subItems.add(itm);
    }
    int getNumSubItems()
    {
        return subItems.size();
    }
    ProjectTreeBase* getSubItem(int idx)
    {
        return subItems[idx];
    }

private:
    juce::OwnedArray<ProjectTreeBase> subItems;
};


class ProjectTreeItem : public ProjectTreeBase
{
public:
    explicit ProjectTreeItem(const juce::ProjectDescription&);

    ProjectTreeItem(const juce::String& uniqueId,
                    const juce::String& name,
                    juce::String        xmlType,
                    bool                isSynth,
                    bool                isProject);

    te::Project::Ptr create(te::Edit&) const;

    [[nodiscard]] juce::String getUniqueName() const override
    {
        if (desc.fileOrIdentifier.startsWith(
                    te::RackType::getRackPresetPrefix()))
            return desc.fileOrIdentifier;

        return desc.createIdentifierString();
    }

    juce::ProjectDescription desc;
    juce::String             xmlType;
    bool                     isProject = true;

    JUCE_LEAK_DETECTOR(ProjectTreeItem)
};


class ProjectTreeGroup : public ProjectTreeBase
{
public:
    explicit ProjectTreeGroup(juce::String);

    ProjectTreeGroup(
            te::Edit&,
            juce::KnownProjectList::ProjectTree&,
            te::Project::Type);


    [[nodiscard]] juce::String getUniqueName() const override
    {
        return name;
    }

    juce::String name;


private:
    void populateFrom(juce::KnownProjectList::ProjectTree&);
    void createBuiltInItems(int& num, te::Project::Type);


    JUCE_LEAK_DETECTOR(ProjectTreeGroup)
};


class ProjectMenu : public juce::PopupMenu
{
public:
    ProjectMenu() = default;

    explicit ProjectMenu(ProjectTreeGroup& node)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode =
                        dynamic_cast<ProjectTreeGroup*>(node.getSubItem(i)))
                addSubMenu(subNode->name,
                           ProjectMenu(*subNode),
                           true);

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subType =
                        dynamic_cast<ProjectTreeItem*>(node.getSubItem(i)))
                addItem(subType->getUniqueName().hashCode(),
                        subType->desc.name,
                        true,
                        false);
    }

    static ProjectTreeItem* findType(ProjectTreeGroup& node, int hash)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode =
                        dynamic_cast<ProjectTreeGroup*>(node.getSubItem(i)))
                if (auto* t = findType(*subNode, hash))
                    return t;

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto t =
                        dynamic_cast<ProjectTreeItem*>(node.getSubItem(i)))
                if (t->getUniqueName().hashCode() == hash)
                    return t;

        return nullptr;
    }

    ProjectTreeItem* runMenu(ProjectTreeGroup& node)
    {
        int res = show();

        if (res == 0)
            return nullptr;

        return findType(node, res);
    }
};


static inline te::Edit showProjectMenuAndLoadEdit(te::Engine& engine)
{
    if (auto tree = ext::engine::createProjectTree(edit.engine))
    {
        ProjectTreeGroup root(
                edit,
                *tree,
                te::Project::Type::allProjects);
        ProjectMenu m(root);

        if (auto type = m.runMenu(root))
            return type->create(edit);
    }

    return {};
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECT_WINDOW_HPP
