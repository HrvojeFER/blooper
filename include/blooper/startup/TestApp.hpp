#ifndef BLOOPER_TEST_APP_HPP
#define BLOOPER_TEST_APP_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_TEST_NAMESPACE_BEGIN

class App : public juce::JUCEApplication
{
 public:
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
  void closeAllModalComponentsAsync(TOnClose onClose);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(App)
  JUCE_DECLARE_WEAK_REFERENCEABLE(App)
};


template<typename TOnClose>
void App::closeAllModalComponentsAsync(TOnClose onClose)
{
  static_assert(
      meta::traits::is_callback(meta::typeid_(onClose)),
      "onClose passed to closeAllModalComponentsAsync has to "
      "satisfy Callback.");


  if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents())
  {
    juce::Timer::callAfterDelay(
        quitRetryIntervalMilliseconds,
        [app = juce::WeakReference<App>(this),
         onClose = move(onClose)]() {
          if (app.wasObjectDeleted()) return;
          app->closeAllModalComponentsAsync(move(onClose));
        });
  }
  else
  {
    onClose();
  }
}

BLOOPER_TEST_NAMESPACE_END

#endif // BLOOPER_TEST_APP_HPP
