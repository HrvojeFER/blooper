#include <blooper/body/panels/project/ProjectContentComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/style.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/body/panels/project/EditComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectContentComponent::Pimpl :
    public juce::TabbedComponent
{
 public:
  explicit Pimpl(ProjectContentComponent* parent)
      : juce::TabbedComponent(
            juce::TabbedButtonBar::Orientation::TabsAtTop),
        parent(parent)
  {
  }

  ~Pimpl() override = default;


  void currentTabChanged(
      int newCurrentTabIndex,
      const juce::String&) override
  {
    this->parent->getContext().setFocusedEdit(
        this->parent->editComponents[newCurrentTabIndex]->getEdit());
  }

  void popupMenuClickOnTab(
      int,
      const juce::String&) override
  {
  }


 private:
  [[maybe_unused]] ProjectContentComponent* parent;
};


ProjectContentComponent::ProjectContentComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      editUpdate(false)
{
  this->tabs =
      std::make_unique<ProjectContentComponent::Pimpl>(
          this);


  ext::addAndMakeVisible(
      *this,
      *this->tabs);


  this->updateEdits();

  this->getContext().getEditManager().addListener(this);
  this->getContext().registerCommandTarget(this);
}

ProjectContentComponent::~ProjectContentComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->getContext().getEditManager().removeListener(this);
}


[[maybe_unused]] void ProjectContentComponent::updateEdits()
{
  this->tabs->clearTabs();
  this->editComponents.clear();

  this->getContext().getEditManager().visit(
      [this](JuceEditRef edit) {
        EditOptions componentOptions{};

        const auto editName = edit->getName();

        auto editChild =
            this->getState()
                .getChildWithProperty(
                    te::IDs::uid,
                    edit->getProjectItemID().getItemID());

        if (!editChild.isValid())
        {
          editChild =
              this->getState()
                  .getOrCreateChildWithName(
                      EditComponent::stateId,
                      nullptr);

          editChild.setProperty(
              te::IDs::uid,
              edit->getProjectItemID().getItemID(),
              nullptr);
        }

        auto editComponent =
            new EditComponent(
                this->getContext(),
                move(editChild),
                std::move(edit),
                move(componentOptions));
        this->editComponents.add(editComponent);

        this->tabs->addTab(
            move(editName),
            this->findColour(
                ColourId::background),
            editComponent,
            false);
      });
}


// Component

void ProjectContentComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->tabs->setBounds(availableArea);
}


// ValueTreeListener

void ProjectContentComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->getContext().getEditManager().getState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->editUpdate);
    }
  }
}

void ProjectContentComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->getContext().getEditManager().getState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->editUpdate);
    }
  }
}

void ProjectContentComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->getContext().getEditManager().getState())
  {
    if (tree.getChild(childAIndex).hasType(id::edit) &&
        tree.getChild(childBIndex).hasType(id::edit))
    {
      this->markAndUpdate(this->editUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void ProjectContentComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(this->editUpdate))
  {
    this->updateEdits();
  }
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget*
ProjectContentComponent::getNextCommandTarget()
{
  return nullptr;
}

void ProjectContentComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void ProjectContentComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool ProjectContentComponent::perform(
    const juce::ApplicationCommandTarget::InvocationInfo& info)
{
  if (info.commandID != CommandId::selectAll) return false;

  if (auto selectionManager =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selectionManager->deselectAll();
    this->getContext().getEditManager().visit(
        [selectionManager](const JuceEditRef& edit) {
          selectionManager->select(*edit, true);
        });
  }

  return true;
}

BLOOPER_NAMESPACE_END
