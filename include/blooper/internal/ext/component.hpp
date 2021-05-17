#ifndef BLOOPER_EXT_COMPONENT_HPP
#define BLOOPER_EXT_COMPONENT_HPP


BLOOPER_EXT_NAMESPACE_BEGIN

namespace comp
{
template<typename... TComponents>
[[maybe_unused]] inline auto addAndMakeVisible(
        juce::Component& to,
        TComponents&... components)
        -> decltype((to.addAndMakeVisible(components), ...))
{
    return (to.addAndMakeVisible(components), ...);
}
} // namespace comp

BLOOPER_EXT_NAMESPACE_END


#endif // BLOOPER_EXT_COMPONENT_HPP
