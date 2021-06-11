#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BrowserPanelComponent::BrowserPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->browser =
      std::make_unique<juce::FileBrowserComponent>(
          juce::FileBrowserComponent::FileChooserFlags::openMode |
              juce::FileBrowserComponent::FileChooserFlags::canSelectFiles,
          JuceString("E:\\audio\\Samples"),
          nullptr,
          nullptr);

  this->browser->addListener(this);


  BrowserPreviewComponent::Options previewOptions{};

  this->preview =
      std::make_unique<BrowserPreviewComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              BrowserPreviewComponent::stateId,
              nullptr),
          move(previewOptions));


  ext::addAndMakeVisible(
      *this,
      *this->browser,
      *this->preview);
}


// Component

void BrowserPanelComponent::paint(JuceGraphics& g)
{
  g.setColour(juce::Colours::whitesmoke);

  g.drawRect(
      this->getLocalBounds().reduced(2),
      2);
}

void BrowserPanelComponent::resized()
{
  auto availableArea = this->getLocalBounds().reduced(6);

  this->browser->setBounds(availableArea);
}


// FileBrowserListener

void BrowserPanelComponent::selectionChanged()
{
}

void BrowserPanelComponent::fileClicked(
    const juce::File&,
    const juce::MouseEvent&)
{
}

void BrowserPanelComponent::fileDoubleClicked(
    const juce::File& file)
{
  this->options.onFileSelected(file);
}

void BrowserPanelComponent::browserRootChanged(
    const juce::File&)
{
}

BLOOPER_NAMESPACE_END
