#ifndef BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP
#define BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_NAMESPACE_BEGIN

struct FilePathPropertyOptions
{
  bool isDir{false};
  bool checkOSPath{true};

  juce::String wildcards{"*"};
  juce::File   root{};
};


// ============================================================================
// Without Enablement

class FilePathPropertyComponent :
    public juce::PropertyComponent,
    public juce::FileDragAndDropTarget,

    protected juce::Value::Listener
{
 public:
  [[maybe_unused]] FilePathPropertyComponent(
      juce::Value             value,
      juce::String            name,
      FilePathPropertyOptions options = {});

  [[maybe_unused]] ~FilePathPropertyComponent() override;


  [[maybe_unused]] FilePathPropertyComponent(
      juce::ValueWithDefault  value,
      juce::String            name,
      FilePathPropertyOptions options = {});


  FilePathPropertyOptions options;


 protected:
  std::unique_ptr<juce::TextPropertyComponent> textProperty;
  std::unique_ptr<juce::TextButton>            browseButton;

  bool highlightedForDragAndDrop;


  void setTo(juce::File f);

  void updateColour();


  // PropertyComponent

 public:
  void refresh() override;


  // Component

 public:
  void resized() override;

  void paintOverChildren(juce::Graphics& g) override;


  // FileDragAndDropTarget

 public:
  bool isInterestedInFileDrag(const juce::StringArray&) override;

  void fileDragEnter(const juce::StringArray&, int, int) override;

  void fileDragExit(const juce::StringArray&) override;

  void filesDropped(const juce::StringArray& selectedFiles, int, int) override;


  // ValueListener

 protected:
  void valueChanged(juce::Value&) override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilePathPropertyComponent)
};


// ============================================================================
// With Enablement

class [[maybe_unused]] FilePathPropertyComponentWithEnablement :
    public FilePathPropertyComponent
{
 public:
  [[maybe_unused]] FilePathPropertyComponentWithEnablement(
      juce::ValueWithDefault  value,
      juce::String            name,
      FilePathPropertyOptions options = {});


 private:
  void updateEnablement();


  // ValueListener

 private:
  void valueChanged(juce::Value& v) override;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_FILE_PATH_PROPERTY_COMPONENT_HPP
