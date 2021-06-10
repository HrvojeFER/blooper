#ifndef BLOOPER_INFO_WINDOW_HPP
#define BLOOPER_INFO_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class InfoWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(InfoWindow);


  struct Options
  {
  } options;

  explicit InfoWindow(
      AbstractContext& context,
      State            state,
      Options          options = {});


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoWindow);
};

[[maybe_unused]] InfoWindow* showInfo(
    AbstractContext&    context,
    InfoWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_INFO_WINDOW_HPP
