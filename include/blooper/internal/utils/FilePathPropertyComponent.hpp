#ifndef BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP
#define BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP


#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

class FilePathPropertyComponent :
    public juce::PropertyComponent,
    public juce::FileDragAndDropTarget,
    protected juce::Value::Listener
{
 public:
  [[maybe_unused]] FilePathPropertyComponent(
      juce::Value         valueToControl,
      const juce::String& propertyName,
      bool                isDir,
      bool                thisOS = true,
      const juce::String& wildcardsToUse = "*",
      const juce::File&   relativeRoot = juce::File())

      : PropertyComponent(propertyName),
        text(
            valueToControl,
            propertyName,
            1024,
            false),

        isDirectory(isDir),
        isThisOS(thisOS),
        wildcards(wildcardsToUse),
        root(relativeRoot)
  {
    textValue.referTo(valueToControl);
    init();
  }

  [[maybe_unused]] FilePathPropertyComponent(
      juce::ValueWithDefault& valueToControl,
      const juce::String&     propertyName,
      bool                    isDir,
      bool                    thisOS = true,
      const juce::String&     wildcardsToUse = "*",
      const juce::File&       relativeRoot = juce::File())
      : PropertyComponent(propertyName),

        text(valueToControl,
             propertyName,
             1024,
             false),

        isDirectory(isDir),
        isThisOS(thisOS),
        wildcards(wildcardsToUse),
        root(relativeRoot)
  {
    textValue = valueToControl.getPropertyAsValue();
    init();
  }


  void refresh() override
  {
  }


  void resized() override
  {
    auto bounds = getLocalBounds();

    text.setBounds(
        bounds.removeFromLeft(
            juce::jmax(
                400,
                bounds.getWidth() - 55)));

    bounds.removeFromLeft(5);

    browseButton.setBounds(bounds);
  }

  void paintOverChildren(juce::Graphics& g) override
  {
    if (highlightForDragAndDrop)
    {
      g.setColour(
          juce::Colours::whitesmoke
              .withAlpha(0.5f));

      g.fillRect(
          getLookAndFeel().getPropertyComponentContentPosition(
              text));
    }
  }


  bool isInterestedInFileDrag(const juce::StringArray&) override
  {
    return true;
  }

  void fileDragEnter(const juce::StringArray&, int, int) override
  {
    highlightForDragAndDrop = true;
    repaint();
  }

  void fileDragExit(const juce::StringArray&) override
  {
    highlightForDragAndDrop = false;
    repaint();
  }

  void filesDropped(const juce::StringArray& selectedFiles, int, int) override
  {
    setTo(selectedFiles[0]);

    highlightForDragAndDrop = false;
    repaint();
  }


 protected:
  void valueChanged(juce::Value&) override
  {
    updateEditorColour();
  }


 private:
  void init()
  {
    textValue.addListener(this);

    text.setInterestedInFileDrag(false);
    addAndMakeVisible(text);

    browseButton.onClick = [this] { browse(); };
    addAndMakeVisible(browseButton);

    lookAndFeelChanged();
  }

  void setTo(juce::File f)
  {
    if (isDirectory && !f.isDirectory())
      f = f.getParentDirectory();

    auto pathName =
        (root == juce::File()) ?
            f.getFullPathName() :
            f.getRelativePathFrom(root);

    text.setText(pathName);
    updateEditorColour();
  }

  void browse()
  {
    juce::File currentFile = {};

    if (text.getText().isNotEmpty())
      currentFile = root.getChildFile(text.getText());

    if (isDirectory)
    {
      juce::FileChooser chooser(
          "Select directory",
          currentFile);

      if (chooser.browseForDirectory())
        setTo(chooser.getResult());
    }
    else
    {
      juce::FileChooser chooser(
          "Select file",
          currentFile,
          wildcards);

      if (chooser.browseForFileToOpen())
        setTo(chooser.getResult());
    }
  }

  void updateEditorColour()
  {
    if (isThisOS)
    {
      auto pathToCheck = text.getText();

      if (pathToCheck.isNotEmpty())
      {
        pathToCheck.replace(
            "${user.home}",
            "~");

#if JUCE_WINDOWS
        if (pathToCheck.startsWith("~"))
          pathToCheck = pathToCheck.replace(
              "~",
              juce::File::getSpecialLocation(
                  juce::File::userHomeDirectory)
                  .getFullPathName());
#endif

        if (!root.getChildFile(pathToCheck).exists())
          text.setColour(
              juce::TextPropertyComponent::textColourId,
              juce::Colours::red);
      }
    }
  }

  void lookAndFeelChanged() override
  {
    browseButton.setColour(
        juce::TextButton::buttonColourId,
        findColour(juce::TextButton::textColourOffId));

    browseButton.setColour(
        juce::TextButton::textColourOffId,
        juce::Colours::white);

    updateEditorColour();
  }


  juce::Value textValue;

  juce::TextPropertyComponent text;
  juce::TextButton            browseButton{"..."};

  bool         isDirectory, isThisOS, highlightForDragAndDrop = false;
  juce::String wildcards;
  juce::File   root;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilePathPropertyComponent)
};


class [[maybe_unused]] FilePathPropertyComponentWithEnablement :
    public FilePathPropertyComponent
{
 public:
  [[maybe_unused]] FilePathPropertyComponentWithEnablement(
      juce::ValueWithDefault& valueToControl,
      juce::ValueWithDefault  valueToListenTo,
      const juce::String&     propertyName,
      bool                    isDir,
      bool                    thisOS = true,
      const juce::String&     wildcardsToUse = "*",
      const juce::File&       relativeRoot = juce::File())
      : FilePathPropertyComponent(
            valueToControl,
            propertyName,
            isDir,
            thisOS,
            wildcardsToUse,
            relativeRoot),

        valueWithDefault(valueToListenTo),
        value(valueToListenTo.getPropertyAsValue())
  {
    value.addListener(this);

    valueChanged(value);
  }

  [[maybe_unused]] ~FilePathPropertyComponentWithEnablement() override
  {
    value.removeListener(this);
  }


 private:
  void valueChanged(juce::Value& v)
  {
    FilePathPropertyComponent::valueChanged(v);
    setEnabled(valueWithDefault.get());
  }


  juce::ValueWithDefault valueWithDefault;
  juce::Value            value;
};

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP
