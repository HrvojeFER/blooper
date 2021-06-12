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
          "Project Panel");

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


  ext::addAndMakeVisible(
      *this,
      *this->viewport);


  ext::referTo(
      this->editScrollRangeStart,
      this->getState(),
      ProjectPanelComponent::editScrollRangeStartId,
      nullptr,
      0);

  ext::referTo(
      this->editScrollRangeEnd,
      this->getState(),
      ProjectPanelComponent::editScrollRangeEndId,
      nullptr,
      0);

  if (this->editScrollRangeStart == 0 && this->editScrollRangeEnd == 0)
  {
    this->viewport->getHorizontalScrollBar()
        .scrollToBottom(
            juce::sendNotificationAsync);
  }
  else
  {
    this->viewport->getHorizontalScrollBar().setCurrentRange(
        {this->editScrollRangeStart.get(),
         this->editScrollRangeEnd.get()},
        juce::sendNotificationAsync);
  }
}

ProjectPanelComponent::~ProjectPanelComponent()
{
  auto currentScrollRange =
      this->viewport->getHorizontalScrollBar()
          .getCurrentRange();

  this->editScrollRangeStart = currentScrollRange.getStart();
  this->editScrollRangeEnd = currentScrollRange.getEnd();
}


// Component

void ProjectPanelComponent::paint(JuceGraphics& g)
{
  g.setColour(juce::Colours::whitesmoke);

  g.drawRect(
      this->getLocalBounds().reduced(2),
      2);
}

void ProjectPanelComponent::resized()
{
  const auto availableArea = this->getLocalBounds().reduced(6);

  ext::setHeight(*this->content, availableArea.getHeight());
  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
