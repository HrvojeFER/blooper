#include <blooper/body/panels/PanelWindow.hpp>

BLOOPER_NAMESPACE_BEGIN

PanelWindow::PanelWindow(
    AbstractContext& context,
    State            state,
    JuceString       name,
    JuceComponent&   content,
    Options          options)
    : WindowBase(
          move(name),
          context,
          move(state)),
      options(move(options)),

      content(content)
{
  this->setContentNonOwned(
      &content,
      true);
}

PanelWindow::~PanelWindow()
{
  delete this;
}


void PanelWindow::closeButtonPressed()
{
  this->options.onClose();
}

BLOOPER_NAMESPACE_END
