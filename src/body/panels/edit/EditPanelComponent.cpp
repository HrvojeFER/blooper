#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditPanelComponent::EditPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  EditContentComponent::Options contentOptions{};

  this->content =
      std::make_unique<EditContentComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              EditContentComponent::stateId,
              nullptr),
          move(contentOptions));


  this->viewport =
      std::make_unique<juce::Viewport>(
          "EditPanel");

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
      projectState.getProperty(EditPanelComponent::editScrollRangeStart),
      projectState.getProperty(EditPanelComponent::editScrollRangeEnd)};

  this->viewport->getHorizontalScrollBar().setCurrentRange(scrollRange);


  ext::addAndMakeVisible(
      *this,
      *this->viewport);
}

EditPanelComponent::~EditPanelComponent()
{
  auto& projectState = this->getContext().getProjectState();

  auto currentScrollRange =
      this->viewport->getHorizontalScrollBar().getCurrentRange();

  projectState.setProperty(
      EditPanelComponent::editScrollRangeStart,
      currentScrollRange.getStart(),
      nullptr);

  projectState.setProperty(
      EditPanelComponent::editScrollRangeEnd,
      currentScrollRange.getEnd(),
      nullptr);
}


void EditPanelComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
