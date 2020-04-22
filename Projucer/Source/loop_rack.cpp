#include "loop_rack.hpp"

namespace blooper
{
	loop_rack::loop_rack(const loop_rack_configuration& configuration)
	{
		for(size_t i = 0 ; i < configuration.loop_group_sizes.size() ; ++i)
		{
			auto* loop_group = new blooper::loop_group
			{
				"loop_group_" + String{ i },
				configuration.loop_group_sizes[i]
			};
			loop_groups_.add(loop_group);
			addAndMakeVisible(loop_group);
		}

		for(auto i = 0 ; i < configuration.number_of_loops ; ++i)
		{
			auto* loop = new blooper::loop{ "loop_" + String{ i } };
			loops_.add(loop);
			addAndMakeVisible(loop);
		}
	}

	void loop_rack::resized()
	{
		const auto bounds = getLocalBounds().reduced(padding);
		outline_ = bounds;

		auto loop_group_area = bounds
			.withTrimmedRight(static_cast<int>(bounds.getWidth() * loop_area_width_factor))
			.reduced(padding);
		loop_group_outline_ = loop_group_area;
		loop_group_area.reduce(padding, padding);
		const auto loop_group_width = loop_group_area.getWidth() / loop_groups_.size();

		for (auto i = 0 ; i < loop_groups_.size() ; ++i)
		{
			loop_groups_[i]->setBounds(
			{
				loop_group_area.getX() + i * loop_group_width,
				loop_group_area.getY(),
				loop_group_width,
				loop_group_area.getHeight()
			});
		}

		auto loop_area = bounds
			.withTrimmedLeft(static_cast<int>(bounds.getWidth() * (1 - loop_area_width_factor)))
			.reduced(padding);
		loop_outline_ = loop_area;
		loop_area.reduce(padding, padding);

		const auto loop_height = loop_area.getHeight() / loops_.size();
		for (auto i = 0 ; i < loops_.size() ; ++i)
		{
			loops_[i]->setBounds(
			{
				loop_area.getX(),
				loop_area.getY() + loop_height * i,
				loop_area.getWidth(),
				loop_height
			});
		}
	}

	void loop_rack::paint(Graphics& g)
	{
		g.setColour(Colours::orange);
		g.drawRect(outline_);

		g.setColour(Colours::magenta);
		g.drawRect(loop_group_outline_);
		g.drawRect(loop_outline_);
	}
}
