#include "loop.hpp"

namespace blooper
{
	// ReSharper disable once CppParameterMayBeConst
	loop::loop(String name) :  // NOLINT(performance-unnecessary-value-param)
		button_
		{
			name,
			// violet/orange color scheme
			Colour::fromRGB(102, 0, 255),
			Colour::fromRGB(153, 51, 255),
			Colour::fromRGB(255, 153, 51)
		},
		state_(state::paused)
	{
		button_.onClick = [&] { toggle(); };

		addAndMakeVisible(button_);
	}

	
	void loop::toggle()
	{
		state_ = next_legal_state.at(state_)[0];
		set_shape(state_shape_[state_]);
	}


	void loop::resized()
	{
		const auto bounds = getLocalBounds().reduced(padding);
		outline_ = bounds;

		auto main_area = bounds
			.withTrimmedRight(static_cast<int>(bounds.getWidth() * main_area_width_factor))
			.reduced(padding);
		main_area_outline_ = main_area;
		main_area.reduce(padding, padding);

		const auto button_area = fit_to_square(main_area);
		state_shape_ = create_state_shape_map(rectangle_to_float(button_area));
		button_.setBounds(button_area);
		set_shape(state_shape_[state_]);
	}

	void loop::paint(Graphics& g)
	{
		g.setColour(Colours::blueviolet);
		g.drawRect(outline_);
		g.drawRect(main_area_outline_);
	}

	[[nodiscard]] Path create_pause_shape(const Rectangle<float>& bounds)
	{
		Path pause;
		pause.addRectangle(bounds);

		return pause;
	}

	[[nodiscard]] Path create_play_shape(const Rectangle<float>& bounds)
	{
		const auto first_point = bounds.getTopLeft();
		const auto second_point = Point<float>
		{
			bounds.getX() + bounds.getWidth(),
			bounds.getCentreY()
		};
		const auto third_point = bounds.getBottomLeft();
		
		Path play;
		play.addTriangle(first_point, second_point, third_point);

		return play;
	}

	[[nodiscard]] std::unordered_map<loop::state, Path> loop::create_state_shape_map(
		const Rectangle<float>& bounds)
	{
		return
		{
			{ state::empty, {} },
			{ state::recording, {} },
			{ state::paused, create_pause_shape(bounds) },
			{ state::playing, create_play_shape(bounds) }
		};
	}

	[[nodiscard]] loop::state_map<std::vector<loop::state>> loop::create_next_legal_state_map()
	{
		return
		{
			{ state::empty, std::vector<state>
				{ state::recording } },
			{ state::recording, std::vector<state>
				{ state::playing, state::paused  } },
			{ state::playing, std::vector<state>
				{ state::paused, state::recording, state::empty  } },
			{ state::paused, std::vector<state>
				{ state::playing, state::recording, state::empty  } }
		};
	}

	const loop::state_map<std::vector<loop::state>> loop::next_legal_state =
		create_next_legal_state_map();

	void loop::set_shape(const Path& shape)
	{
		button_.setShape(shape, false, true, false);
	}

}
