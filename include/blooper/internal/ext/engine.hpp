#ifndef BLOOPER_EXT_ENGINE_HPP
#define BLOOPER_EXT_ENGINE_HPP


BLOOPER_EXT_NAMESPACE_BEGIN

namespace engine
{
static inline void showAudioDeviceSettings(te::Engine& engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour =
            juce::LookAndFeel::getDefaultLookAndFeel()
                    .findColour(juce::ResizableWindow::backgroundColourId);

    o.content.setOwned(
            new juce::AudioDeviceSelectorComponent(
                    engine.getDeviceManager().deviceManager,
                    0,
                    512,
                    1,
                    512,
                    false,
                    false,
                    true,
                    true));

    o.content->setSize(400, 600);
    o.launchAsync();
}

static inline void showPluginSettings(te::Engine& engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Plugins");
    o.dialogBackgroundColour = juce::Colours::black;
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = true;
    o.useBottomRightCornerResizer = true;

    auto v = new juce::PluginListComponent(
            engine.getPluginManager().pluginFormatManager,
            engine.getPluginManager().knownPluginList,
            engine.getTemporaryFileManager()
                    .getTempFile("PluginScanDeadMansPedal"),
            te::getApplicationSettings());

    v->setSize(800, 600);
    o.content.setOwned(v);
    o.launchAsync();
}

static inline void showProjectSelector(te::Engine& engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Select project...");
    o.dialogBackgroundColour = juce::Colours::black;
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = true;
    o.useBottomRightCornerResizer = true;


    auto& projectManager = engine.getProjectManager();
    auto  projects = projectManager.getAllProjects(
            projectManager.getActiveProjectsFolder());


    v->setSize(800, 600);
    o.content.setOwned(v);
    o.launchAsync();
}

static inline std::unique_ptr<juce::KnownPluginList::PluginTree>
createPluginTree(te::Engine& engine)
{
    auto& list = engine.getPluginManager().knownPluginList;

    if (auto tree = juce::KnownPluginList::createTree(
                list.getTypes(),
                juce::KnownPluginList::sortByManufacturer))
        return tree;

    return {};
}
} // namespace engine

BLOOPER_EXT_NAMESPACE_END


#endif // BLOOPER_EXT_ENGINE_HPP
