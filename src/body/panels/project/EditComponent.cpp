#include <blooper/body/panels/project/EditComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/value_tree.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/body/panels/project/EditContentComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

EditComponent::EditComponent(
    AbstractContext& context,
    State            state,
    JuceEditRef      edit,
    EditOptions      options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      edit(std::move(edit))
{
  EditContentOptions contentOptions{};

  this->content =
      std::make_unique<EditContentComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              EditContentComponent::stateId,
              nullptr),
          this->edit,
          move(contentOptions));


  this->viewport =
      std::make_unique<juce::Viewport>(
          this->edit->getName());

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
      this->editScrollStart,
      this->getState(),
      EditComponent::editScrollStartId,
      nullptr,
      0);

  ext::referTo(
      this->editScrollEnd,
      this->getState(),
      EditComponent::editScrollEndId,
      nullptr,
      0);

  if (this->editScrollStart == 0 && this->editScrollEnd == 0)
  {
    this->viewport->getHorizontalScrollBar()
        .scrollToBottom(
            juce::sendNotificationAsync);
  }
  else
  {
    this->viewport->getHorizontalScrollBar().setCurrentRange(
        {this->editScrollStart.get(),
         this->editScrollEnd.get()},
        juce::sendNotificationAsync);
  }
}

EditComponent::~EditComponent()
{
  auto currentScrollRange =
      this->viewport->getHorizontalScrollBar()
          .getCurrentRange();

  this->editScrollStart = currentScrollRange.getStart();
  this->editScrollEnd = currentScrollRange.getEnd();
}


// Component

void EditComponent::paint(JuceGraphics& g)
{
  util::drawOutline(
      g,
      *this);
}

void EditComponent::resized()
{
  auto availableArea =
      util::pad(
          this->getLocalBounds(),
          outlinePaddingFactor);

  ext::setHeight(*this->content, availableArea.getHeight());
  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
