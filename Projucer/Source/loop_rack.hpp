#ifndef BLOOPER_LOOP_RACK_HPP
#define BLOOPER_LOOP_RACK_HPP

#include "pch/pch.hpp"

#include "loop_group.hpp"
#include "loop.hpp"

namespace blooper
{
	struct loop_rack_configuration
	{
		const std::vector<int> loop_group_sizes;
		const int number_of_loops;
	};
	
	class loop_rack final : public Component
	{
	public:
		explicit loop_rack(const loop_rack_configuration& configuration);

		void resized() override;

		void paint(Graphics& g) override;
	private:
		static constexpr float loop_area_width_factor{ 0.3f };
		static constexpr int padding{ 10 };
		
		OwnedArray<loop_group> loop_groups_{};
		OwnedArray<loop> loops_{};

		Rectangle<int> outline_{};
		Rectangle<int> loop_group_outline_{};
		Rectangle<int> loop_outline_{};
	};
}

#endif
