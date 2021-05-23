#ifndef BLOOPER_PLUGIN_WINDOW_HPP
#define BLOOPER_PLUGIN_WINDOW_HPP

#include <blooper/internal/internal.hpp>

// TODO: cleanup

BLOOPER_NAMESPACE_BEGIN

#if JUCE_LINUX
constexpr bool shouldAddPluginWindowToDesktop = false;
#else
constexpr bool shouldAddPluginWindowToDesktop = true;
#endif

class PluginEditor : public juce::Component
{
public:
    virtual bool                              allowWindowResizing() = 0;
    virtual juce::ComponentBoundsConstrainer* getBoundsConstrainer() = 0;
};


struct AudioProcessorEditorContentComp : public PluginEditor
{
    explicit AudioProcessorEditorContentComp(te::ExternalPlugin& plug)
        : plugin(plug)
    {
        JUCE_AUTORELEASEPOOL
        {
            if (auto pi = plugin.getAudioPluginInstance())
            {
                editor.reset(pi->createEditorIfNeeded());

                if (editor == nullptr)
                    editor = std::make_unique<juce::GenericAudioProcessorEditor>(
                            *pi);

                addAndMakeVisible(*editor);
            }
        }

        resizeToFitEditor(true);
    }

    bool allowWindowResizing() override
    {
        return false;
    }

    juce::ComponentBoundsConstrainer* getBoundsConstrainer() override
    {
        if (editor == nullptr || allowWindowResizing())
            return {};

        return editor->getConstrainer();
    }

    void resized() override
    {
        if (editor != nullptr)
            editor->setBounds(getLocalBounds());
    }

    void childBoundsChanged(Component* c) override
    {
        if (c == editor.get())
        {
            plugin.edit.pluginChanged(plugin);
            resizeToFitEditor();
        }
    }

    void resizeToFitEditor(bool force = false)
    {
        if (force || !allowWindowResizing())
            setSize(juce::jmax(
                            8,
                            editor != nullptr ? editor->getWidth() : 0),
                    juce::jmax(
                            8,
                            editor != nullptr ? editor->getHeight() : 0));
    }

    te::ExternalPlugin&                         plugin;
    std::unique_ptr<juce::AudioProcessorEditor> editor;

    AudioProcessorEditorContentComp() = delete;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessorEditorContentComp)
};


class PluginWindow : public juce::DocumentWindow
{
public:
    explicit PluginWindow(te::Plugin&);
    ~PluginWindow() override;

    static std::unique_ptr<Component> create(te::Plugin&);

    void show();

    void                        setEditor(std::unique_ptr<PluginEditor>);
    [[nodiscard]] PluginEditor* getEditor() const
    {
        return editor.get();
    }

    void recreateEditor();
    void recreateEditorAsync();

private:
    void moved() override;
    void userTriedToCloseWindow() override
    {
        plugin.windowState->closeWindowExplicitly();
    }
    void closeButtonPressed() override
    {
        userTriedToCloseWindow();
    }
    [[nodiscard]] float getDesktopScaleFactor() const override
    {
        return 1.0f;
    }

    std::unique_ptr<PluginEditor> createContentComp();

    std::unique_ptr<PluginEditor> editor;

    te::Plugin&            plugin;
    te::PluginWindowState& windowState;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_WINDOW_HPP
