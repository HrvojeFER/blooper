#include "pch/pch.hpp"


#include "main_component.hpp"

#include "loop.hpp"


namespace blooper
{

    main_component::main_component()
    {
        setSize(800, 600);

        if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
            && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
        {
            RuntimePermissions::request(
                RuntimePermissions::recordAudio,
                [&](const bool& granted)
                {
                    if (granted) setAudioChannels(2, 2);
                });
        }
        else
        {
            setAudioChannels(2, 2);
        }

        addAndMakeVisible(loop_rack_);
    }

    main_component::~main_component()
    {
        // This shuts down the audio device and clears the audio source.
        shutdownAudio();
    }


    void main_component::prepareToPlay(int /*samples_per_block_expected*/, double /*sample_rate*/)
    {

    }

    void main_component::getNextAudioBlock(const AudioSourceChannelInfo& buffer_to_fill)
    {
        buffer_to_fill.clearActiveBufferRegion();
    }

    void main_component::releaseResources()
    {

    }


    void main_component::paint(Graphics& graphics)
    {
        graphics.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
    }

    void main_component::resized()
    {
        loop_rack_.setBounds(getLocalBounds());
    }
}
