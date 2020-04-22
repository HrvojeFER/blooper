#include "pch/pch.hpp"


#include "main_component.hpp"


namespace blooper
{
    class app final : public JUCEApplication
    {
    public:
        app() = default;

        // ReSharper disable once CppConstValueFunctionReturnType
        const String getApplicationName() override { return ProjectInfo::projectName; }
        // ReSharper disable once CppConstValueFunctionReturnType
        const String getApplicationVersion() override { return ProjectInfo::versionString; }
        bool moreThanOneInstanceAllowed() override { return true; }

    	
        void initialise(const String& /*command_line*/) override
        {
            // This method is where you should put your application's initialisation code..

            main_window_.reset(new main_window(getApplicationName()));
        }

        void shutdown() override
        {
            // Add your application's shutdown code here..

            main_window_ = nullptr; // (deletes our window)
        }

    	
        void systemRequestedQuit() override
        {
            quit();
        }

        // The command_line parameter tells you what
        // the other instance's command-line arguments were.
        void anotherInstanceStarted(const String& /*command_line*/) override
        {
        	
        }

    	
        class main_window final : public DocumentWindow
        {
        public:
            explicit main_window(const String name) : DocumentWindow(
                name,
                Desktop::getInstance().getDefaultLookAndFeel().findColour(backgroundColourId), allButtons)
            {
                setUsingNativeTitleBar(true);
                setContentOwned(new main_component(), true);

#if JUCE_IOS || JUCE_ANDROID
                setFullScreen(true);
#else
                setResizable(true, true);
                centreWithSize(getWidth(), getHeight());
#endif

                Component::setVisible(true);
            }

            void closeButtonPressed() override
            {
                getInstance()->systemRequestedQuit();
            }

            /* Note: Be careful if you override any DocumentWindow methods - the base
               class uses a lot of them, so by overriding you might break its functionality.
               It's best to do all your work in your content component instead, but if
               you really have to override any DocumentWindow methods, make sure your
               subclass also calls the superclass's method.
            */

        private:
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(main_window)
        };

    private:
        std::unique_ptr<main_window> main_window_;
    };
}


// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (blooper::app)
