#include <blooper/context/behaviour/PropertyStorage.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/string.hpp>

BLOOPER_NAMESPACE_BEGIN

PropertyStorage::PropertyStorage(
    JuceString           name,
    AbstractCoreContext& context)
    : CoreContextualBase(context),

      te::PropertyStorage(move(name))
{
  auto& rootDir = this->getContext().getRootDir();

  this->cache =
      std::make_unique<juce::File>(
          rootDir.getChildFile(cacheDirName));
  if (!this->cache->exists()) this->cache->createDirectory();

  this->media =
      std::make_unique<juce::File>(
          rootDir.getChildFile(mediaDirName));
  if (!this->media->exists()) this->media->createDirectory();

  this->prefs =
      std::make_unique<juce::File>(
          rootDir.getChildFile(prefsDirName));
  if (!this->prefs->exists()) this->prefs->createDirectory();
}


[[maybe_unused]] JuceState
PropertyStorage::getProperties()
{
  return this->getContext()
      .getEngineSettings()
      .getOrCreateChildWithName(
          id::properties,
          nullptr);
}

JuceState
PropertyStorage::getChild(
    te::SettingID childId)
{
  auto undoManager = this->getContext().getUndoManagerPtr();

  const auto idVar = JuceVar(te::PropertyStorage::settingToString(childId));

  auto properties = this->getProperties();
  auto child = properties.getChildWithProperty(id::name, idVar);

  if (!child.isValid())
  {
    child = JuceState{id::property};
    child.setProperty(id::name, idVar, nullptr);
    properties.addChild(child, -1, undoManager);
  }

  return child;
}

JuceState
PropertyStorage::getGrandchild(
    te::SettingID   childId,
    juce::StringRef grandchildName)
{
  auto undoManager = this->getContext().getUndoManagerPtr();

  const auto itemVar = ext::toVar(grandchildName);

  auto child = this->getChild(childId);
  auto grandchild = child.getChildWithProperty(id::name, itemVar);

  if (!grandchild.isValid())
  {
    grandchild = JuceState{id::property};
    grandchild.setProperty(id::name, itemVar, nullptr);
    child.addChild(grandchild, -1, undoManager);
  }

  return grandchild;
}


// PropertyStorage

juce::File PropertyStorage::getAppCacheFolder()
{
  return *this->cache;
}

juce::File PropertyStorage::getAppPrefsFolder()
{
  return *this->prefs;
}


void PropertyStorage::flushSettingsToDisk()
{
  this->getContext().saveEngineSettings();
}


void PropertyStorage::removeProperty(te::SettingID id)
{
  auto child = this->getChild(id);

  child.getParent().removeChild(
      child,
      this->getContext().getUndoManagerPtr());
}

juce::var PropertyStorage::getProperty(
    te::SettingID    id,
    const juce::var& defaultValue)
{
  return this->getChild(id)
      .getProperty(
          id::value,
          defaultValue);
}

void PropertyStorage::setProperty(
    te::SettingID    id,
    const juce::var& value)
{
  this->getChild(id)
      .setProperty(
          id::value,
          value,
          this->getContext().getUndoManagerPtr());
}


std::unique_ptr<juce::XmlElement> PropertyStorage::getXmlProperty(
    te::SettingID id)
{
  return juce::parseXML(
      this->getChild(id)
          .getProperty(
              id::value,
              ""));
}

void PropertyStorage::setXmlProperty(
    te::SettingID           id,
    const juce::XmlElement& xml)
{
  this->getChild(id)
      .setProperty(
          id::value,
          xml.toString(
              JuceXml::TextFormat{}.withoutHeader().singleLine()),
          this->getContext().getUndoManagerPtr());
}


void PropertyStorage::removePropertyItem(
    te::SettingID   id,
    juce::StringRef item)
{
  auto grandchild = this->getGrandchild(id, item);

  grandchild.getParent().removeChild(
      grandchild,
      this->getContext().getUndoManagerPtr());
}

juce::var PropertyStorage::getPropertyItem(
    te::SettingID    id,
    juce::StringRef  item,
    const juce::var& defaultValue)
{
  return this->getGrandchild(id, item)
      .getProperty(
          id::value,
          defaultValue);
}

void PropertyStorage::setPropertyItem(
    te::SettingID    id,
    juce::StringRef  item,
    const juce::var& value)
{
  this->getGrandchild(id, item)
      .setProperty(
          id::value,
          value,
          this->getContext().getUndoManagerPtr());
}


std::unique_ptr<juce::XmlElement> PropertyStorage::getXmlPropertyItem(
    te::SettingID   id,
    juce::StringRef item)
{
  return juce::parseXML(
      this->getGrandchild(id, item)
          .getProperty(
              id::value,
              ""));
}

void PropertyStorage::setXmlPropertyItem(
    te::SettingID           id,
    juce::StringRef         item,
    const juce::XmlElement& xml)
{
  this->getGrandchild(id, item)
      .setProperty(
          id::value,
          xml.toString(
              JuceXml::TextFormat{}.withoutHeader().singleLine()),
          this->getContext().getUndoManagerPtr());
}


juce::File PropertyStorage::getDefaultLoadSaveDirectory(
    juce::StringRef)
{
  // TODO: figure this out
  return *this->media;
}

void PropertyStorage::setDefaultLoadSaveDirectory(
    juce::StringRef,
    const juce::File& newPath)
{
  // TODO: figure this out
  *this->media = newPath;
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
  return blooper::appName;
}

juce::String PropertyStorage::getApplicationVersion()
{
  return blooper::appVersion;
}

BLOOPER_NAMESPACE_END
