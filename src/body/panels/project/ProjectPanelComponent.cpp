#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectPanelComponent::ProjectPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
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


  this->viewport =
      std::make_unique<juce::Viewport>(
          "ProjectPanel");

  this->viewport->setLookAndFeel(
      std::addressof(this->getLookAndFeel()));

  this->viewport->setViewedComponent(
      this->content.get(),
      false);

  this->viewport->setScrollBarsShown(
      false,
      true,
      false,
      false);

  this->viewport->setScrollOnDragEnabled(
      true);

  auto& projectState = this->getContext().getProjectState();

  juce::Range<double> scrollRange{
      projectState.getProperty(ProjectPanelComponent::editScrollRangeStart),
      projectState.getProperty(ProjectPanelComponent::editScrollRangeEnd)};

  this->viewport->getHorizontalScrollBar().setCurrentRange(scrollRange);


  ext::addAndMakeVisible(
      *this,
      *this->viewport);
}

ProjectPanelComponent::~ProjectPanelComponent()
{
  auto& projectState = this->getContext().getProjectState();

  auto currentScrollRange =
      this->viewport->getHorizontalScrollBar().getCurrentRange();

  projectState.setProperty(
      ProjectPanelComponent::editScrollRangeStart,
      currentScrollRange.getStart(),
      nullptr);

  projectState.setProperty(
      ProjectPanelComponent::editScrollRangeEnd,
      currentScrollRange.getEnd(),
      nullptr);
}


void ProjectPanelComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
