#include "loop_group.hpp"

namespace blooper
{
	// ReSharper disable once CppParameterMayBeConst
	loop_group::loop_group(String name, const int number_of_loops) :  // NOLINT(performance-unnecessary-value-param)
		name_{ name }
	{
		for (auto i = 0; i < number_of_loops; ++i)
		{
			auto* loop = new blooper::loop{ name + "_loop_" + String { i } };
			loops_.add(loop);
			addAndMakeVisible(loop);
		}
	}
	
	void loop_group::resized()
	{
		const auto bounds = getLocalBounds().reduced(padding);
		outer_outline_ = bounds;

		auto main_area = bounds
			.withTrimmedBottom(static_cast<int>(bounds.getHeight() * (1 - main_area_height_factor)))
			.reduced(padding);
		main_area_outline_ = main_area;
		main_area.reduce(padding, padding);

		auto loop_area = bounds
			.withTrimmedTop(static_cast<int>(bounds.getHeight() * main_area_height_factor))
			.reduced(padding);
		loop_area_outline_ = loop_area;
		loop_area.reduce(padding, padding);
		
		const auto loop_height = loop_area.getHeight() / loops_.size();
		for (auto i = 0 ; i < loops_.size() ; ++i)
		{
			loops_[i]->setBounds(
			{
				loop_area.getX(),
				loop_area.getY() + i * loop_height,
				loop_area.getWidth(),
				loop_height
			});
		}
	}

	void loop_group::paint(Graphics& g)
	{
		g.setColour(Colours::violet);
		g.drawRect(outer_outline_);
		g.drawRect(main_area_outline_);
		g.drawRect(loop_area_outline_);
	}
}
