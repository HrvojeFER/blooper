#ifndef BLOOPER_DEV_WINDOW_HPP
#define BLOOPER_DEV_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(DevWindow);


  struct Options
  {
  } options;

  explicit DevWindow(
      AbstractContext& context,
      State            state,
      Options          options = {});


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevWindow);
};

[[maybe_unused]] DevWindow* showDev(
    AbstractContext&   context,
    DevWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_WINDOW_HPP
