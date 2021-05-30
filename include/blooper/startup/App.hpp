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


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(App)
  JUCE_DECLARE_WEAK_REFERENCEABLE(App)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_APP_HPP
