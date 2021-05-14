#ifndef BLOOPER_BODY_COMPONENT_HPP
#define BLOOPER_BODY_COMPONENT_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>
#include <blooper/context/context.hpp>

#include <blooper/body/panels/panels.hpp>

#include <blooper/body/header/header.hpp>
#include <blooper/body/footer/footer.hpp>


BLOOPER_NAMESPACE_BEGIN

class BodyComponent : public juce::Component
{
public:
    BodyComponent();
    ~BodyComponent() override;


    void resized() override;


private:
    PanelsComponent panels;

    HeaderComponent header;
    FooterComponent footer;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_BODY_COMPONENT_HPP
