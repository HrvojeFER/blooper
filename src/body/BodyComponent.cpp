#include <blooper/body/BodyComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

BodyComponent::BodyComponent()
    : panels(),
      header(),
      footer()
{
    addAndMakeVisible(panels);

    addAndMakeVisible(header);
    addAndMakeVisible(footer);
}

BodyComponent::~BodyComponent() = default;


void BodyComponent::resized()
{
    auto       unusedArea = getLocalBounds();
    const auto bodyHeight = unusedArea.getHeight();

    header.setBounds(
            unusedArea.removeFromTop(
                    int(bodyHeight * 0.1)));
    footer.setBounds(
            unusedArea.removeFromBottom(
                    int(bodyHeight * 0.05)));

    panels.setBounds(unusedArea);
}

BLOOPER_NAMESPACE_END
