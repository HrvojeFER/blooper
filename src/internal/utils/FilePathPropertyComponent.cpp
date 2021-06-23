#include <blooper/internal/utils/FilePathPropertyComponent.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

// ============================================================================
// Without Enablement

[[maybe_unused]] FilePathPropertyComponent::FilePathPropertyComponent(
    juce::Value             value,
    juce::String            name,
    FilePathPropertyOptions options)
    : PropertyComponent(move(name)),
      options(move(options)),

      highlightedForDragAndDrop(false)
{
  this->textProperty =
      std::make_unique<juce::TextPropertyComponent>(
          move(value),
          this->getName(),
          1024,
          false);

  this->textProperty->setInterestedInFileDrag(
      false);


  this->browseButton =
      std::make_unique<juce::TextButton>(
          "...");

  this->browseButton->onClick = [this] {
    juce::File currentFile{};
    if (auto text = this->textProperty->getText(); text.isNotEmpty())
      currentFile = this->options.root.getChildFile(text);

    const auto chooserTitle =
        this->options.isDir ?
            "Select directory" :
            "Select file";

    juce::FileChooser chooser =
        this->options.isDir ?

            juce::FileChooser{
                move(chooserTitle),
                move(currentFile)} :

            juce::FileChooser{
                move(chooserTitle),
                move(currentFile),
                this->options.wildcards};

    if (this->options.isDir ?
            chooser.browseForDirectory() :
            chooser.browseForFileToOpen())
      this->setTo(chooser.getResult());
  };


  ext::addAndMakeVisible(
      *this,
      *this->textProperty,
      *this->browseButton);


  this->textProperty->getValue().addListener(this);
}

FilePathPropertyComponent::~FilePathPropertyComponent()
{
  this->textProperty->getValue().removeListener(this);
}


[[maybe_unused]] FilePathPropertyComponent::FilePathPropertyComponent(
    juce::ValueWithDefault  valueToControl,
    juce::String            propertyName,
    FilePathPropertyOptions options)
    : FilePathPropertyComponent(
          valueToControl.getPropertyAsValue(),
          move(propertyName),
          move(options))
{
}


void FilePathPropertyComponent::setTo(juce::File file)
{
  if (this->options.isDir && !file.isDirectory())
    file = file.getParentDirectory();

  this->textProperty->setText(
      (this->options.root == juce::File()) ?
          file.getFullPathName() :
          file.getRelativePathFrom(file));

  this->updateColour();
}

void FilePathPropertyComponent::updateColour()
{
  if (this->options.checkOSPath)
  {
    auto text = this->textProperty->getText();

    if (text.isNotEmpty())
    {
      text = text.replace(
          "${user.home}",
          "~");

#if JUCE_WINDOWS
      if (text.startsWith("~"))
        text = text.replace(
            "~",
            juce::File::getSpecialLocation(
                juce::File::userHomeDirectory)
                .getFullPathName());
#endif

      if (!this->options.root.getChildFile(text).exists())
        this->textProperty->setColour(
            juce::TextPropertyComponent::textColourId,
            this->findColour(
                juce::AlertWindow::textColourId));
    }
  }
}


// PropertyComponent

void FilePathPropertyComponent::refresh()
{
}


// Component

void FilePathPropertyComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->textProperty->setBounds(
      availableArea.removeFromLeft(
          juce::jmax(
              400,
              availableArea.getWidth() - 55)));

  availableArea.removeFromLeft(5);
  this->browseButton->setBounds(availableArea);
}

void FilePathPropertyComponent::paintOverChildren(juce::Graphics& g)
{
  if (this->highlightedForDragAndDrop)
  {
    g.setColour(
        juce::Colours::whitesmoke
            .withAlpha(0.5f));

    g.fillRect(
        this->getLookAndFeel()
            .getPropertyComponentContentPosition(
                *this->textProperty));
  }
}


// FileDragAndDropTarget

bool FilePathPropertyComponent::isInterestedInFileDrag(
    const juce::StringArray&)
{
  return true;
}

void FilePathPropertyComponent::fileDragEnter(
    const juce::StringArray&,
    int,
    int)
{
  this->highlightedForDragAndDrop = true;
  this->repaint();
}

void FilePathPropertyComponent::fileDragExit(
    const juce::StringArray&)
{
  this->highlightedForDragAndDrop = false;
  this->repaint();
}

void FilePathPropertyComponent::filesDropped(
    const juce::StringArray& selectedFiles,
    int,
    int)
{
  this->setTo(selectedFiles[0]);

  this->highlightedForDragAndDrop = false;
  this->repaint();
}


// ValueListener

void FilePathPropertyComponent::valueChanged(juce::Value&)
{
  // no way to effectively check if two juce::Value's have the same source...
  this->updateColour();
}


// ============================================================================
// With Enablement

[[maybe_unused]] FilePathPropertyComponentWithEnablement::
    FilePathPropertyComponentWithEnablement(
        juce::ValueWithDefault  value,
        juce::String            name,
        FilePathPropertyOptions options)
    : FilePathPropertyComponent(
          move(value),
          move(name),
          move(options))
{
  this->updateEnablement();
}


void FilePathPropertyComponentWithEnablement::updateEnablement()
{
  // juce naming ugh
  this->setEnabled(this->textProperty->getValue().getValue());
}


// ValueListener

void FilePathPropertyComponentWithEnablement::valueChanged(juce::Value& value)
{
  FilePathPropertyComponent::valueChanged(value);

  // no way to effectively check if two juce::Value's have the same source...
  this->updateEnablement();
}

BLOOPER_UTIL_NAMESPACE_END
