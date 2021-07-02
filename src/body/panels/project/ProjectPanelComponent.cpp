#include <blooper/body/panels/project/ProjectPanelComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/body/panels/project/ProjectContentComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectPanelComponent::ProjectPanelComponent(
    AbstractContext&    context,
    State               state,
    ProjectPanelOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  ProjectContentComponent::Options contentOptions{};

  this->content =
      std::make_unique<ProjectContentComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              ProjectContentComponent::stateId,
              nullptr),
          move(contentOptions));


  ext::addAndMakeVisible(
      *this,
      *this->content);
}

ProjectPanelComponent::~ProjectPanelComponent() = default;


// Component

void ProjectPanelComponent::paint(JuceGraphics& g)
{
  util::drawOutline(
      g,
      *this);
}

void ProjectPanelComponent::resized()
{
  auto availableArea =
      util::pad(
          this->getLocalBounds(),
          outlinePaddingFactor);

  this->content->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
