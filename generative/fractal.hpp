#ifndef GENERATIVE_FRACTAL_HPP
#define GENERATIVE_FRACTAL_HPP

#include <cassert>
#include <cmath>

namespace generative
{
	template <typename NoiseType, typename Vector>
	typename Vector::value_type
	fractal(
		NoiseType noise,
		unsigned int octaves,
		typename Vector::value_type lacunarity,
		typename Vector::value_type persistence,
		const Vector& position)
	{
		typename Vector::value_type sum = 0, per = 1, lac = 1;
		for (unsigned int i = 0; i < octaves; i++, per *= persistence, lac *= lacunarity)
			sum += noise(position * lac) * per;
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
	
	template <typename NoiseType, typename Vector>
	typename Vector::value_type
	fractal_feedback(
		NoiseType noise,
		unsigned int octaves,
		typename Vector::value_type lacunarity,
		typename Vector::value_type persistence,
		const Vector& position)
	{
		typename Vector::value_type sum = 0, per = 1, lac = 1, prev = 1;
		for (unsigned int i = 0; i < octaves; i++, per *= persistence, lac *= lacunarity)
		{
			typename Vector::value_type current = noise(position * lac) * per;
			sum += current * prev;
			prev = current;
		}
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
	
	inline unsigned int fractal_octaves(float persistence, float precision)
	{
		return (unsigned int)std::ceil(std::log(precision) / std::log(persistence));
	}
	
	inline float fractal_offset(unsigned int octaves, float lacunarity, float length = 1)
	{
		return length * (1 - std::pow(lacunarity, octaves - 1)) / (1 - lacunarity);
	}
}
#endif
