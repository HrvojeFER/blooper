#include <blooper/context/behaviour/PropertyStorage.hpp>

#include <blooper/context/core/core.hpp>

#include <blooper/context/devices/devices.hpp>
#include <blooper/context/settings/settings.hpp>
#include <blooper/context/plugins/plugins.hpp>
#include <blooper/context/projects/projects.hpp>


BLOOPER_NAMESPACE_BEGIN

PropertyStorage::PropertyStorage(CoreContext& context)
    : te::PropertyStorage(JUCE_APPLICATION_NAME_STRING),
      context(context),

      root(createRoot()),
      cache(createCache()),
      prefs(createPrefs()),

      media(createMedia()),
      projects(createProjects()),

      properties(createProperties())
{
    if (!root.exists()) root.createDirectory();
    if (!cache.exists()) cache.createDirectory();
    if (!prefs.exists()) prefs.createDirectory();

    if (!media.exists()) media.createDirectory();
    if (!projects.exists()) projects.createDirectory();

    auto& propertiesFile = properties.getFile();
    if (!propertiesFile.exists()) propertiesFile.create();
}

PropertyStorage::~PropertyStorage() = default;


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
    properties.save();
}


void PropertyStorage::removeProperty(te::SettingID setting)
{
    properties.removeValue(
            te::PropertyStorage::settingToString(setting));
}

juce::var PropertyStorage::getProperty(
        te::SettingID    setting,
        const juce::var& defaultValue)
{
    return properties.getValue(
            te::PropertyStorage::settingToString(setting),
            defaultValue);
}

void PropertyStorage::setProperty(
        te::SettingID    setting,
        const juce::var& value)
{
    properties.setValue(
            PropertyStorage::settingToString(setting),
            value);
}

std::unique_ptr<juce::XmlElement> PropertyStorage::getXmlProperty(
        te::SettingID setting)
{
    return std::unique_ptr<juce::XmlElement>(
            properties.getXmlValue(
                    te::PropertyStorage::settingToString(setting)));
}

void PropertyStorage::setXmlProperty(
        te::SettingID           setting,
        const juce::XmlElement& xml)
{
    properties.setValue(
            te::PropertyStorage::settingToString(setting),
            &xml);
}


void PropertyStorage::removePropertyItem(
        te::SettingID   setting,
        juce::StringRef item)
{
    properties.removeValue(
            te::PropertyStorage::settingToString(setting) +
            "_" +
            item);
}

juce::var PropertyStorage::getPropertyItem(
        te::SettingID    setting,
        juce::StringRef  item,
        const juce::var& defaultValue)
{
    return properties.getValue(
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
    properties.setValue(
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
            properties.getXmlValue(
                    te::PropertyStorage::settingToString(setting) +
                    "_" +
                    item));
}

void PropertyStorage::setXmlPropertyItem(
        te::SettingID           setting,
        juce::StringRef         item,
        const juce::XmlElement& xml)
{
    properties.setValue(
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
    return media;
}

void PropertyStorage::setDefaultLoadSaveDirectory(
        juce::StringRef   label,
        const juce::File& newPath)
{
    juce::ignoreUnused(label);

    // TODO: figure this out
    media = newPath;
}

juce::File PropertyStorage::getDefaultLoadSaveDirectory(
        te::ProjectItem::Category category)
{
    // TODO: figure this out
    switch (category)
    {
        case te::ProjectItem::Category::exports:
            return projects.getChildFile("exports");

        case te::ProjectItem::Category::imported:
            return projects.getChildFile("imports");

        case te::ProjectItem::Category::archives:
            return projects.getChildFile("archives");


        case te::ProjectItem::Category::edit:
            return projects.getChildFile("edits");

        case te::ProjectItem::Category::recorded:
            return projects.getChildFile("records");

        case te::ProjectItem::Category::frozen:
            return projects.getChildFile("freezer");

        case te::ProjectItem::Category::rendered:
            return projects.getChildFile("renders");

        case te::ProjectItem::Category::video:
            return projects.getChildFile("videos");


        default:
            return projects.getChildFile("other");
    }
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
