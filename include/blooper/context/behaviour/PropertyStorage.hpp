#ifndef BLOOPER_PROPERTY_STORAGE_HPP
#define BLOOPER_PROPERTY_STORAGE_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PropertyStorage :
    public CoreContextualBase,
    public te::PropertyStorage
{
 public:
  inline constexpr static auto rootSpecialLocation =
      juce::File::userHomeDirectory;

  inline constexpr static auto rootDirName = ".blooper";
  inline constexpr static auto cacheDirName = ".cache";
  inline constexpr static auto prefsDirName = "prefs";

  inline constexpr static auto projectsDirName = "projects";
  inline constexpr static auto mediaDirName = "media";

  inline constexpr static auto settingsFileName = "settings.xml";


  explicit PropertyStorage(AbstractCoreContext& context);


  juce::File getAppCacheFolder() override;

  juce::File getAppPrefsFolder() override;


  void flushSettingsToDisk() override;


  void removeProperty(
      te::SettingID setting_id) override;

  juce::var getProperty(
      te::SettingID    setting,
      const juce::var& defaultValue) override;

  void setProperty(
      te::SettingID    setting,
      const juce::var& value) override;

  std::unique_ptr<juce::XmlElement> getXmlProperty(
      te::SettingID setting) override;

  void setXmlProperty(
      te::SettingID           setting,
      const juce::XmlElement& element) override;


  void removePropertyItem(
      te::SettingID   setting,
      juce::StringRef item) override;

  juce::var getPropertyItem(
      te::SettingID    setting,
      juce::StringRef  item,
      const juce::var& defaultValue) override;

  void setPropertyItem(
      te::SettingID    setting,
      juce::StringRef  item,
      const juce::var& value) override;

  std::unique_ptr<juce::XmlElement> getXmlPropertyItem(
      te::SettingID   setting,
      juce::StringRef item) override;

  void setXmlPropertyItem(
      te::SettingID           setting,
      juce::StringRef         item,
      const juce::XmlElement& element) override;


  juce::File getDefaultLoadSaveDirectory(
      juce::StringRef label) override;

  void setDefaultLoadSaveDirectory(
      juce::StringRef   label,
      const juce::File& newPath) override;

  juce::File getDefaultLoadSaveDirectory(
      te::ProjectItem::Category category) override;


  juce::String getApplicationName() override;

  juce::String getApplicationVersion() override;


 private:
  juce::File
      root,
      cache,
      prefs,

      media,
      projects;

  juce::PropertiesFile
      properties;


  static inline juce::File createRoot();


  inline juce::File createCache();

  inline juce::File createPrefs();


  inline juce::File createMedia();

  inline juce::File createProjects();


  inline juce::PropertiesFile ensureValidProperties();


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyStorage)
};


juce::File PropertyStorage::createRoot()
{
  const auto appDataRoot = juce::File::getSpecialLocation(rootSpecialLocation);

  return appDataRoot.getChildFile(rootDirName);
}

juce::File PropertyStorage::createCache()
{
  return this->root.getChildFile(cacheDirName);
}

juce::File PropertyStorage::createPrefs()
{
  return this->root.getChildFile(prefsDirName);
}


juce::File PropertyStorage::createMedia()
{
  return this->root.getChildFile(mediaDirName);
}

juce::File PropertyStorage::createProjects()
{
  return this->root.getChildFile(projectsDirName);
}


juce::PropertiesFile PropertyStorage::ensureValidProperties()
{
  juce::PropertiesFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = juce::PropertiesFile::storeAsXML;
  options.commonToAllUsers = false;

  auto file = this->root.getChildFile(settingsFileName);

  // If not valid, just delete it and it will create a new one when needed.
  if (!juce::PropertiesFile(file, options).isValidFile())
    file.deleteFile();

  return juce::PropertiesFile(file, options);
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROPERTY_STORAGE_HPP
