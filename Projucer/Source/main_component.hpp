#ifndef BLOOPER_MAIN_COMPONENT_HPP
#define BLOOPER_MAIN_COMPONENT_HPP

#include "pch/pch.hpp"

#include "loop_rack.hpp"

namespace blooper
{
    class main_component final : public AudioAppComponent
    {
    public:
        main_component();
        ~main_component();

        main_component(main_component&&) = delete;
        main_component& operator =(main_component&&) = delete;

        void prepareToPlay(int, double) override;
        void getNextAudioBlock(const AudioSourceChannelInfo&) override;
        void releaseResources() override;

        void paint(Graphics&) override;
        void resized() override;

    private:
        loop_rack loop_rack_
    	{
    		{
    			{ 2, 3 },
    			3
    		}
    	};
    	
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(main_component)
    };	
}

#endif
