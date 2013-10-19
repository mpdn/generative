#ifndef GENERATIVE_SEAMLESS_HPP
#define GENERATIVE_SEAMLESS_HPP

#include <cmath>
#include <glm/glm.hpp>

namespace generative
{
	namespace
	{
		//These internal functions are neccesary to avoid relying on glm implementation details
		
		template <typename Vector, typename FloatType>
		Vector seamless_internal(FloatType periodicity, FloatType value)
		{
			const FloatType tau = (FloatType)6.283185307179586;
			FloatType s = value / periodicity * tau;
			return Vector(std::cos(s), std::sin(s)) / tau;
		}
	
		template <typename BigVector, typename SmallVector, typename FloatType>
		BigVector seamless_internal(FloatType periodicity, const SmallVector& vector)
		{
			const FloatType tau = (FloatType)6.283185307179586;
			FloatType s = vector.x / periodicity * tau,
				      t = vector.y / periodicity * tau;
			return BigVector(std::cos(s),
				             std::cos(t),
				             std::sin(s),
				             std::sin(t)) / tau;
		}
	}
	
	inline glm::vec2 seamless(float periodicity, float value)
	{
		return seamless_internal<glm::vec2, float>(periodicity, value);
	}
	
	inline glm::dvec2 seamless(double periodicity, double value)
	{
		return seamless_internal<glm::dvec2, double>(periodicity, value);
	}
	
	inline glm::vec4 seamless(float periodicity, glm::vec2 value)
	{
		return seamless_internal<glm::vec4, glm::vec2, float>(periodicity, value);
	}
	
	inline glm::dvec4 seamless(double periodicity, glm::dvec2 value)
	{
		return seamless_internal<glm::dvec4, glm::dvec2, double>(periodicity, value);
	}
}

#endif // GENERATIVE_SEAMLESS_HPP

