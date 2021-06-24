#include <blooper/body/BodyWindow.hpp>

#include <blooper/body/BodyComponent.hpp>
#include <blooper/body/BodyMenuBar.hpp>

BLOOPER_NAMESPACE_BEGIN

BodyWindow::BodyWindow(
    AbstractContext& context,
    State            state,
    JuceString       name,
    Options          options)
    : WindowBase(
          move(name),
          context,
          move(state)),
      options(move(options))
{
  BodyComponent::Options componentOptions{};

  component =
      std::make_unique<BodyComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              BodyComponent::stateId,
              nullptr),
          move(componentOptions));

  this->setContentNonOwned(
      this->component.get(),
      false);


  BodyMenuBar::Options menuBarOptions{};

  menuBar =
      std::make_unique<BodyMenuBar>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              BodyMenuBar::stateId,
              nullptr),
          move(menuBarOptions));

  this->setMenuBar(
      menuBar.get(),
      this->getLookAndFeel()
          .getDefaultMenuBarHeight());


  if (this->windowState->isEmpty())
  {
    setFullScreen(true);
  }
}

BodyWindow::~BodyWindow()
{
  this->setMenuBar(
      nullptr,
      this->getLookAndFeel()
          .getDefaultMenuBarHeight());
}


// Window

[[maybe_unused]] void BodyWindow::closeButtonPressed()
{
  this->options.onClose();
}

BLOOPER_NAMESPACE_END
