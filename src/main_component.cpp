#include <blooper/blooper.hpp>
#include <blooper/main_component.hpp>


namespace blooper
{
main_component::main_component()
{
    setSize(800, 600);
    setLookAndFeel(new audio::LookAndFeel_V4());

    _welcome_label.setEditable(true);
    addAndMakeVisible(_welcome_label);
}

void main_component::paint(audio::Graphics&)
{
}

void main_component::resized()
{
    _welcome_label.setBounds(5, 5, 200, 200);
}
} // namespace blooper
