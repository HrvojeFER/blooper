#ifndef BLOOPER_PROPERTY_STORAGE_HPP
#define BLOOPER_PROPERTY_STORAGE_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PropertyStorage :
    public CoreContextualBase,
    public te::PropertyStorage
{
 public:
  inline constexpr static auto cacheDirName = ".cache";
  inline constexpr static auto prefsDirName = "prefs";

  inline constexpr static auto mediaDirName = "media";


  inline constexpr static auto propertiesKey = "properties";

  BLOOPER_ID(propertiesId);


  explicit PropertyStorage(
      JuceString           name,
      AbstractCoreContext& context);


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


  juce::String getUserName() override;

  juce::String getApplicationName() override;

  juce::String getApplicationVersion() override;


 private:
  std::unique_ptr<juce::File>
      cache,
      prefs,

      media;


  [[maybe_unused]] JuceState
  getProperties();

  [[maybe_unused]] JuceState
  getChild(te::SettingID childId);

  [[maybe_unused]] JuceState
  getGrandchild(te::SettingID childId, juce::StringRef grandchildName);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyStorage)
};


BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROPERTY_STORAGE_HPP
