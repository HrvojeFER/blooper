#ifndef BLOOPER_BROWSER_COMPONENT_HPP
#define BLOOPER_BROWSER_COMPONENT_HPP


BLOOPER_NAMESPACE_BEGIN

class BrowserComponent :
    public juce::Component,
    private juce::FileBrowserListener
{
public:
    BrowserComponent();

    ~BrowserComponent() override = default;


    void resized() override;


    std::function<void(const juce::File&)> onFileSelected;


private:
    juce::FileBrowserComponent browser;


    friend class juce::FileBrowserComponent;

    void selectionChanged() override;

    void fileClicked(
            const juce::File&       file,
            const juce::MouseEvent& e) override;

    void fileDoubleClicked(const juce::File& file) override;

    void browserRootChanged(const juce::File& newRoot) override;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_BROWSER_COMPONENT_HPP
