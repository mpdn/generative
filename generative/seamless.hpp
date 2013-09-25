#ifndef GENERATIVE_SEAMLESS_HPP
#define GENERATIVE_SEAMLESS_HPP

#include <cmath>
#include <glm/glm.hpp>

namespace generative
{
	template <typename FloatType>
	glm::detail::tvec2<FloatType> seamless(FloatType periodicity, FloatType value)
	{
		const FloatType tau = (FloatType)6.283185307179586;
		FloatType s = value / periodicity * tau;
		return glm::detail::tvec2<FloatType>(std::cos(s), std::sin(s)) / tau;
	}
	
	template <typename FloatType>
	glm::detail::tvec4<FloatType> seamless(FloatType periodicity, const glm::detail::tvec2<FloatType>& vector)
	{
		const FloatType tau = (FloatType)6.283185307179586;
		FloatType s = vector.x / periodicity * tau,
		          t = vector.y / periodicity * tau;
		return glm::detail::tvec4<FloatType>(std::cos(s),
		                                     std::cos(t),
		                                     std::sin(s),
		                                     std::sin(t)) / tau;
	}
}

#endif // GENERATIVE_SEAMLESS_HPP

