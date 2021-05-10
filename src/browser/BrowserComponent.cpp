#include <blooper/browser/BrowserComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

BrowserComponent::BrowserComponent()
    : browser(juce::FileBrowserComponent::FileChooserFlags::openMode |
                      juce::FileBrowserComponent::FileChooserFlags::canSelectFiles,
              {"E:\\audio\\Samples"},
              nullptr,
              nullptr),
      onFileSelected([](const juce::File&) {})
{
    browser.addListener(this);
    addAndMakeVisible(browser);
}

void BrowserComponent::resized()
{
    const auto r = getLocalBounds();
    browser.setBounds(r);
}


void BrowserComponent::selectionChanged()
{
}

void BrowserComponent::fileClicked(const juce::File&, const juce::MouseEvent&)
{
}

void BrowserComponent::fileDoubleClicked(const juce::File& file)
{
    onFileSelected(file);
}

void BrowserComponent::browserRootChanged(const juce::File&)
{
}

BLOOPER_NAMESPACE_END
