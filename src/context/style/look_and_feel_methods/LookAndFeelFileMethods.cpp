#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::AttributedString LookAndFeel::createFileChooserHeaderText(
    const juce::String& title,
    const juce::String& instructions)
{
  return LookAndFeel_V2::createFileChooserHeaderText(
      title, instructions);
}


void LookAndFeel::layoutFileBrowserComponent(
    juce::FileBrowserComponent&              component,
    juce::DirectoryContentsDisplayComponent* display_component,
    juce::FilePreviewComponent*              preview_component,
    juce::ComboBox*                          currentPathBox,
    juce::TextEditor*                        filenameBox,
    juce::Button*                            goUpButton)
{
  LookAndFeel_V4::layoutFileBrowserComponent(
      component,
      display_component,
      preview_component,
      currentPathBox,
      filenameBox,
      goUpButton);
}

void LookAndFeel::drawFileBrowserRow(
    juce::Graphics&                          graphics,
    int                                      width,
    int                                      height,
    const juce::File&                        file,
    const juce::String&                      filename,
    juce::Image*                             icon,
    const juce::String&                      fileSizeDescription,
    const juce::String&                      fileTimeDescription,
    bool                                     isDirectory,
    bool                                     isItemSelected,
    int                                      itemIndex,
    juce::DirectoryContentsDisplayComponent& component)
{
  LookAndFeel_V4::drawFileBrowserRow(
      graphics,
      width,
      height,
      file,
      filename,
      icon,
      fileSizeDescription,
      fileTimeDescription,
      isDirectory,
      isItemSelected,
      itemIndex,
      component);
}

juce::Button* LookAndFeel::createFileBrowserGoUpButton()
{
  return LookAndFeel_V4::createFileBrowserGoUpButton();
}


void LookAndFeel::layoutFilenameComponent(
    juce::FilenameComponent& component,
    juce::ComboBox*          filenameBox,
    juce::Button*            browseButton)
{
  LookAndFeel_V2::layoutFilenameComponent(
      component, filenameBox, browseButton);
}

juce::Button* LookAndFeel::createFilenameComponentBrowseButton(
    const juce::String& text)
{
  return LookAndFeel_V2::createFilenameComponentBrowseButton(
      text);
}


const juce::Drawable* LookAndFeel::getDefaultFolderImage()
{
  return LookAndFeel_V2::getDefaultFolderImage();
}

const juce::Drawable* LookAndFeel::getDefaultDocumentFileImage()
{
  return LookAndFeel_V2::getDefaultDocumentFileImage();
}

BLOOPER_NAMESPACE_END
