#include <blooper/components/tracks/TrackClipsDetailWindow.hpp>

#include <blooper/internal/ext/value_tree.hpp>

#include <blooper/components/tracks/TrackClipsDetailComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackClipsDetailWindow::TrackClipsDetailWindow(
    AbstractContext&              context,
    State                         state,
    JuceTrackRef                  track,
    TrackClipsDetailWindowOptions options)
    : WindowBase(
          track->edit.getName() + " " +
              track->getName() + " " +
              "Clips",
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
  this->component =
      makeChild<TrackClipsDetailComponent>(
          *this,
          this->track);

  this->setContentNonOwned(
      this->component.get(),
      false);
}

TrackClipsDetailWindow::~TrackClipsDetailWindow() = default;


// Show

TrackClipsDetailWindow* showTrackClipsDetail(
    AbstractContext&              context,
    JuceTrackRef                  track,
    TrackClipsDetailWindowOptions options)
{
  // here because the C++ standard does not guarantee
  // the order of argument construction
  auto windowChild =
      ext::findOrCreate<VisitDepth::shallow>(
          context.getProjectState(),
          [&track](const juce::ValueTree& child) {
            return child.hasType(TrackClipsDetailWindow::stateId) &&
                   (child.getProperty(te::IDs::uid) ==
                    track->itemID.getRawID());
          },
          [&context, &track] {
            juce::ValueTree child{TrackClipsDetailWindow::stateId};

            child.setProperty(
                te::IDs::uid,
                track->itemID.getRawID(),
                nullptr);

            context.getProjectState()
                .addChild(
                    child,
                    -1,
                    nullptr);

            return move(child);
          });

  auto window =
      new TrackClipsDetailWindow(
          context,
          move(windowChild),
          move(track),
          move(options));

  window->setVisible(true);

  return window;
}

BLOOPER_NAMESPACE_END
