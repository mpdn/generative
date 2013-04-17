#ifndef COHERENT_MATH_HPP
#define COHERENT_MATH_HPP

#include <cmath>

namespace coherent
{
	template <typename FloatType>
	FloatType fabs(FloatType value)
	{
		return std::fabs(value * 2 - 1);
	}
	
	template <typename FloatType>
	FloatType invert(FloatType value)
	{
		return 1 - value;
	}
	
	template <typename FloatType>
	FloatType square(FloatType value)
	{
		return value * value;
	}
}

#endif
