#ifndef GENERATIVE_MATH_HPP
#define GENERATIVE_MATH_HPP

#include <cmath>

namespace generative
{
	/// Given a value in the range [0,1], returns the absolute value, treating
	/// 0.5 as 0. Eg. 0 will become 1, 0.5 will become 0, and values above 0.5
	/// will not be changed.
	template <typename FloatType>
	FloatType fabs(FloatType value)
	{
		detail::assert_scalar<FloatType>();
		return std::fabs(value * 2 - 1);
	}
	
	/// Given a value in the range [0,1], returns the value in the opposite side
	/// of the range. Eg. 0 will become 1 and vice versa.
	template <typename FloatType>
	FloatType invert(FloatType value)
	{
		detail::assert_scalar<FloatType>();
		return 1 - value;
	}
	
	///
	/// Given any value, returns the value multiplied by itself.
	template <typename FloatType>
	FloatType square(FloatType value)
	{
		detail::assert_scalar<FloatType>();
		return value * value;
	}
}

#endif
