#ifndef BLOOPER_PROPERTY_STORAGE_HPP
#define BLOOPER_PROPERTY_STORAGE_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;


class PropertyStorage : public te::PropertyStorage
{
public:
    inline constexpr auto rootDataDirName = ".blooper";
    inline constexpr auto cacheDirName = ".cache";
    inline constexpr auto prefsDirName = "prefs";

    inline constexpr auto projectsDirName = "projects";
    inline constexpr auto mediaDirName = "media";

    inline constexpr auto settingsFileName = "settings.xml";


    PropertyStorage(CoreContext& context);
    ~PropertyStorage() override;


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
    CoreContext& context;

    juce::File
            root,
            cache,
            prefs,

            media,
            projects;

    juce::PropertiesFile
            properties;


    inline juce::File createRoot();

    inline juce::File createCache();

    inline juce::File createPrefs();


    inline juce::File createMedia();

    inline juce::File createProjects();


    inline juce::PropertiesFile createProperties();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyStorage)
};


juce::File PropertyStorage::createRoot()
{
    juce::ignoreUnused(this);

    const auto appDataRoot = juce::File::getSpecialLocation(
            juce::File::userHomeDirectory);

    return appDataRoot.getChildFile(rootDirName);
}

juce::File PropertyStorage::createCache()
{
    juce::ignoreUnused(this);

    return root.getChildFile(cacheDirName);
}

juce::File PropertyStorage::createPrefs()
{
    juce::ignoreUnused(this);

    return root.getChildFile(prefsDirName);
}


juce::File PropertyStorage::createMedia()
{
    juce::ignoreUnused(this);

    return root.getChildFile(mediaDirName);
}

juce::File PropertyStorage::createProjects()
{
    juce::ignoreUnused(this);

    return root.getChildFile(projectsDirName);
}


juce::PropertiesFile PropertyStorage::createProperties()
{
    juce::ignoreUnused(this);

    juce::PropertiesFile::Options options;
    options.millisecondsBeforeSaving = 2000;
    options.storageFormat = juce::PropertiesFile::storeAsXML;

    return juce::PropertiesFile(
            root.getChildFile(settingsFileName),
            options);
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROPERTY_STORAGE_HPP
