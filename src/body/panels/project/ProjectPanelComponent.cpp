#include <blooper/body/panels/project/ProjectPanelComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/value_tree.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/body/panels/project/ProjectContentComponent.hpp>

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
      this->projectScrollStart,
      this->getState(),
      ProjectPanelComponent::projectScrollStartId,
      nullptr,
      0);

  ext::referTo(
      this->projectScrollEnd,
      this->getState(),
      ProjectPanelComponent::projectScrollEndId,
      nullptr,
      0);

  if (this->projectScrollStart == 0 && this->projectScrollEnd == 0)
  {
    this->viewport->getHorizontalScrollBar()
        .scrollToBottom(
            juce::sendNotificationAsync);
  }
  else
  {
    this->viewport->getHorizontalScrollBar().setCurrentRange(
        {this->projectScrollStart.get(),
         this->projectScrollEnd.get()},
        juce::sendNotificationAsync);
  }
}

ProjectPanelComponent::~ProjectPanelComponent()
{
  auto currentScrollRange =
      this->viewport->getHorizontalScrollBar()
          .getCurrentRange();

  this->projectScrollStart = currentScrollRange.getStart();
  this->projectScrollEnd = currentScrollRange.getEnd();
}


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

  ext::setHeight(*this->content, availableArea.getHeight());
  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
