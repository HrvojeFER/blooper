#ifndef BLOOPER_LOOP_HPP
#define BLOOPER_LOOP_HPP


#include "pch/pch.hpp"

// ReSharper disable once CppUnusedIncludeDirective
#include "utils.hpp"

namespace blooper
{
	class loop final : public Component  // NOLINT(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
	{
	public:
		enum class state
		{
			empty,
			recording,
			playing,
			paused
		};

		explicit loop(String name);
		
		void resized() override;

		void paint(Graphics& g) override;
	private:
		static constexpr float main_area_width_factor{ 0.5f };
		static constexpr int padding{ 10 };
		
		template<typename OutType>
		using state_map = std::unordered_map<state, OutType>;

		static const state_map<std::vector<state>> next_legal_state;
		[[nodiscard]] static state_map<std::vector<state>> create_next_legal_state_map();

		state_map<Path> state_shape_{};
		[[nodiscard]] static state_map<Path> create_state_shape_map(const Rectangle<float>& bounds);

		ShapeButton button_;
		void set_shape(const Path& shape);

		Rectangle<int> outline_{};
		Rectangle<int> main_area_outline_{};
		
		state state_{ state::empty };

		void toggle();

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(loop)
	};
}

#endif
