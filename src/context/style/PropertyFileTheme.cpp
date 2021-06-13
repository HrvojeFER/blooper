#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PropertyFileTheme::PropertyFileTheme(const JuceFile& originalFile)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = true;

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(originalFile, options).isValidFile())
    originalFile.deleteFile();

  this->file = std::make_unique<JuceXmlFile>(originalFile, options);
}

PropertyFileTheme::PropertyFileTheme(const JuceString& path)
    : PropertyFileTheme(JuceFile{path})
{
}


bool PropertyFileTheme::hasColour(JuceColourId id) const
{
  return this->file->containsKey(getColourName(id));
}

JuceColour PropertyFileTheme::getColour(JuceColourId id) const
{
  const auto colourXml = this->file->getXmlValue(getColourName(id));
  if (!colourXml) return JuceColour{};

  return JuceColour{
      static_cast<juce::uint8>(colourXml->getIntAttribute("red")),
      static_cast<juce::uint8>(colourXml->getIntAttribute("green")),
      static_cast<juce::uint8>(colourXml->getIntAttribute("blue")),
      static_cast<juce::uint8>(colourXml->getIntAttribute("alpha"))};
}

BLOOPER_NAMESPACE_END
