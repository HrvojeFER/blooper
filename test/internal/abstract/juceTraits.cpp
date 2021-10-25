#include <env/test.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_TEST_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(juceTraits, state)
{
  EXPECT_TRUE(
      isJuceState(
          meta::type_c<juce::ValueTree>));

  EXPECT_TRUE(
      isJuceChangeBroadcaster(
          meta::type_c<juce::ChangeBroadcaster>));
}

ENV_TEST_CASE(juceTraits, command)
{
  EXPECT_TRUE(
      isJuceCommandTarget(
          meta::type_c<juce::ApplicationCommandTarget>));
}

ENV_TEST_CASE(juceTraits, style)
{
  EXPECT_TRUE(
      isJuceLookAndFeel(
          meta::type_c<juce::LookAndFeel_V4>));
}

ENV_TEST_CASE(juceTraits, component)
{
  EXPECT_TRUE(
      isJuceComponent(
          meta::type_c<juce::Component>));
}

ENV_TEST_CASE(juceTraits, edit)
{
  EXPECT_TRUE(
      isJuceTrack(
          meta::type_c<te::Track>));
}

ENV_TEST_CASE(juceTraits, clip)
{
  EXPECT_TRUE(
      isJuceClip(
          meta::type_c<te::Clip>));

  EXPECT_TRUE(
      isJuceClip(
          meta::type_c<te::WaveAudioClip>));

  EXPECT_TRUE(
      isJuceClip(
          meta::type_c<te::MidiClip>));
}

ENV_TEST_CASE(juceTraits, plugin)
{
  EXPECT_TRUE(
      isJucePlugin(
          meta::type_c<te::Plugin>));

  EXPECT_TRUE(
      isJucePlugin(
          meta::type_c<te::ExternalPlugin>));

  EXPECT_TRUE(
      isJucePluginRef(
          meta::type_c<juce::ReferenceCountedObjectPtr<te::Plugin>>));

  EXPECT_TRUE(
      isJucePluginConstRef(
          meta::type_c<juce::ReferenceCountedObjectPtr<te::Plugin>>));

  EXPECT_TRUE(
      isJucePluginRef(
          meta::type_c<juce::ReferenceCountedObjectPtr<te::ExternalPlugin>>));

  EXPECT_TRUE(
      isJucePluginConstRef(
          meta::type_c<juce::ReferenceCountedObjectPtr<te::ExternalPlugin>>));
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_TEST_NAMESPACE_END
