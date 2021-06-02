#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PropertyStorage::PropertyStorage(
    JuceString           name,
    AbstractCoreContext& context)
    : CoreContextualBase(context),

      te::PropertyStorage(move(name)),

      cache(this->getContext()
                .getRootDir()
                .getChildFile(
                    PropertyStorage::cacheDirName)),
      prefs(this->getContext()
                .getRootDir()
                .getChildFile(
                    PropertyStorage::prefsDirName)),

      media(this->getContext()
                .getRootDir()
                .getChildFile(
                    PropertyStorage::mediaDirName)),
{
  if (!cache.exists()) cache.createDirectory();
  if (!prefs.exists()) prefs.createDirectory();

  if (!media.exists()) media.createDirectory();
}


juce::File PropertyStorage::getAppCacheFolder()
{
  return cache;
}

juce::File PropertyStorage::getAppPrefsFolder()
{
  return prefs;
}


void PropertyStorage::flushSettingsToDisk()
{
  this->getContext().getProperties().save();
}


void PropertyStorage::removeProperty(te::SettingID setting)
{
  this->getContext().getProperties().removeValue(
      te::PropertyStorage::settingToString(setting));
}

juce::var PropertyStorage::getProperty(
    te::SettingID    setting,
    const juce::var& defaultValue)
{
  return this->getContext().getProperties().getValue(
      te::PropertyStorage::settingToString(setting),
      defaultValue);
}

void PropertyStorage::setProperty(
    te::SettingID    setting,
    const juce::var& value)
{
  this->getContext().getProperties().setValue(
      PropertyStorage::settingToString(setting),
      value);
}

std::unique_ptr<juce::XmlElement> PropertyStorage::getXmlProperty(
    te::SettingID setting)
{
  return std::unique_ptr<juce::XmlElement>(
      this->getContext().getProperties().getXmlValue(
          te::PropertyStorage::settingToString(setting)));
}

void PropertyStorage::setXmlProperty(
    te::SettingID           setting,
    const juce::XmlElement& xml)
{
  this->getContext().getProperties().setValue(
      te::PropertyStorage::settingToString(setting),
      &xml);
}


void PropertyStorage::removePropertyItem(
    te::SettingID   setting,
    juce::StringRef item)
{
  this->getContext().getProperties().removeValue(
      te::PropertyStorage::settingToString(setting) +
      "_" +
      item);
}

juce::var PropertyStorage::getPropertyItem(
    te::SettingID    setting,
    juce::StringRef  item,
    const juce::var& defaultValue)
{
  return this->getContext().getProperties().getValue(
      te::PropertyStorage::settingToString(setting) +
          "_" +
          item,
      defaultValue);
}

void PropertyStorage::setPropertyItem(
    te::SettingID    setting,
    juce::StringRef  item,
    const juce::var& value)
{
  this->getContext().getProperties().setValue(
      te::PropertyStorage::settingToString(setting) +
          "_" +
          item,
      value);
}

std::unique_ptr<juce::XmlElement> PropertyStorage::getXmlPropertyItem(
    te::SettingID   setting,
    juce::StringRef item)
{
  return std::unique_ptr<juce::XmlElement>(
      this->getContext().getProperties().getXmlValue(
          te::PropertyStorage::settingToString(setting) +
          "_" +
          item));
}

void PropertyStorage::setXmlPropertyItem(
    te::SettingID           setting,
    juce::StringRef         item,
    const juce::XmlElement& xml)
{
  this->getContext().getProperties().setValue(
      te::PropertyStorage::settingToString(setting) +
          "_" +
          item,
      &xml);
}


juce::File PropertyStorage::getDefaultLoadSaveDirectory(
    juce::StringRef label)
{
  juce::ignoreUnused(label);

  // TODO: figure this out
  return this->media;
}

void PropertyStorage::setDefaultLoadSaveDirectory(
    juce::StringRef   label,
    const juce::File& newPath)
{
  juce::ignoreUnused(label);

  // TODO: figure this out
  this->= newPath;
}

juce::File PropertyStorage::getDefaultLoadSaveDirectory(
    te::ProjectItem::Category category)
{
  JuceFile& projectsDir = this->getContext().getProjectsDir();

  // TODO: figure this out
  switch (category)
  {
    case te::ProjectItem::Category::exports:
      return projectsDir.getChildFile("exports");

    case te::ProjectItem::Category::imported:
      return projectsDir.getChildFile("imports");

    case te::ProjectItem::Category::archives:
      return projectsDir.getChildFile("archives");


    case te::ProjectItem::Category::edit:
      return projectsDir.getChildFile("edits");

    case te::ProjectItem::Category::recorded:
      return projectsDir.getChildFile("records");

    case te::ProjectItem::Category::frozen:
      return projectsDir.getChildFile("freezer");

    case te::ProjectItem::Category::rendered:
      return projectsDir.getChildFile("renders");

    case te::ProjectItem::Category::video:
      return projectsDir.getChildFile("videos");


    default:
      return projectsDir.getChildFile("other");
  }
}


juce::String PropertyStorage::getUserName()
{
  return te::PropertyStorage::getUserName();
}

juce::String PropertyStorage::getApplicationName()
{
  return JUCE_APPLICATION_NAME_STRING;
}

juce::String PropertyStorage::getApplicationVersion()
{
  return JUCE_APPLICATION_VERSION_STRING;
}

BLOOPER_NAMESPACE_END
