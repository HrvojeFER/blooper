#ifndef BLOOPER_APP_HPP
#define BLOOPER_APP_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class App : public juce::JUCEApplication
{
 public:
  inline constexpr static auto quitRetryIntervalMilliseconds = 100;


  App();

  ~App() override;


  const juce::String getApplicationName() override;

  const juce::String getApplicationVersion() override;


  bool moreThanOneInstanceAllowed() override;


  void initialise(const juce::String&) override;

  void anotherInstanceStarted(const juce::String&) override;


  void systemRequestedQuit() override;

  void shutdown() override;


 private:
  std::unique_ptr<class Context> context;

  std::unique_ptr<class BodyWindow> bodyWindow;


  template<typename TOnClose>
  void closeAllModalWindowsAsync(TOnClose onClose);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(App)
  JUCE_DECLARE_WEAK_REFERENCEABLE(App)
};


template<typename TOnClose>
void App::closeAllModalWindowsAsync(TOnClose onClose)
{
  static_assert(
      isCallback(meta::typeid_(onClose)),
      "onClose passed to closeAllModalWindowsAsync has to satisfy Callback.");


  if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents())
  {
    juce::Timer::callAfterDelay(
        App::quitRetryIntervalMilliseconds,
        [app = juce::WeakReference<App>(this),
         onClose = move(onClose)]() {
          if (app.wasObjectDeleted()) return;
          app->closeAllModalWindowsAsync(move(onClose));
        });
  }
  else
  {
    onClose();
  }
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_APP_HPP
