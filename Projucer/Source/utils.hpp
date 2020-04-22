#ifndef BLOOPER_UTILS_HPP
#define BLOOPER_UTILS_HPP

#include "pch/pch.hpp"

namespace blooper
{
	template<typename ValueType>
	[[nodiscard]] Rectangle<float> rectangle_to_float(const Rectangle<ValueType>& rectangle)
	{
		return Rectangle<float>
		{
			static_cast<float>(rectangle.getX()),
				static_cast<float>(rectangle.getY()),
				static_cast<float>(rectangle.getWidth()),
				static_cast<float>(rectangle.getHeight())
		};
	}
	
	template<typename ValueType>
	[[nodiscard]] Rectangle<ValueType> fit_to_square(const Rectangle<ValueType>& rectangle)
	{
		return rectangle.getWidth() > rectangle.getHeight() ?
			Rectangle<ValueType>
		{
			rectangle.getX(),
				rectangle.getY(),
				rectangle.getHeight(),
				rectangle.getHeight()
		} :
		Rectangle<ValueType>
		{
			rectangle.getX(),
			rectangle.getY(),
			rectangle.getWidth(),
			rectangle.getWidth()
		};
	}
}

#endif
