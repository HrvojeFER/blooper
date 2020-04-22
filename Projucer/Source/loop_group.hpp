#ifndef BLOOPER_LOOP_GROUP_HPP
#define BLOOPER_LOOP_GROUP_HPP

#include "pch/pch.hpp"

#include "loop.hpp"

namespace blooper
{
	class loop_group final : public Component
	{
	public:
		enum class state
		{
			empty,
			playing,
			paused
		};

		explicit loop_group(String name, int number_of_loops);

		void resized() override;

		void paint(Graphics& g) override;
	private:
		static constexpr float main_area_height_factor{ 0.3f };
		static constexpr int padding{ 5 };
		
		const String name_;
		OwnedArray<loop> loops_{};

		Rectangle<int> outer_outline_{};
		Rectangle<int> loop_area_outline_{};
		Rectangle<int> main_area_outline_{};

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(loop_group)
	};
}

#endif
