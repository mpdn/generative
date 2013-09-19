#ifndef COHERENT_FRACTAL_HPP
#define COHERENT_FRACTAL_HPP

#include <cassert>
#include <generative/vector_traits.hpp>
#include <glm/glm.hpp>

namespace generative
{
	/// Calculates the sum of several noise functions (called octaves) with
	/// rising frequency and decreasing amplitude. The range of the return type
	/// will be the same as the noise functions (usually [0,1]).
	/// 
	/// @param begin the beginning of the list of noise functions
	/// @param end the end of the list of noise functions
	/// @param lacunarity the proportional change in frequency (a value of 2 will
	/// give each octave double the frequency of the one before it).
	/// @param persistence is the the proportional change in amplitude (a value of 0.5 will give each octave half the amplitude of the one before it). The function will return NaN if persistence is exactly 1.0.
	/// @param position the position of the noise value to return.
	template <typename Iterator, typename Vector>
	typename Vector::value_type
	fractal(Iterator begin,
	        Iterator end,
	        typename Vector::value_type lacunarity,
	        typename Vector::value_type persistence,
	        const Vector& position,
	        typename Vector::value_type precision = 0)
	{
		detail::assert_vector<Vector>();
		
		typename Vector::value_type sum = 0, per = 1, lac = 1;
		for (Iterator i = begin; i != end && per > precision; i++, per *= persistence, lac *= lacunarity)
			sum += (*i)(position * lac) * per;
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
	
	/// Calculates the sum of several noise functions (called octaves) with
	/// rising frequency and decreasing amplitude, multiplied by the previous
	/// octave.
	/// 
	/// @param begin the beginning of the list of noise functions
	/// @param end the end of the list of noise functions
	/// @param lacunarity the proportional change in frequency (a value of 2 will give each octave double the frequency of the one before it).
	/// @param persistence is the the proportional change in amplitude (a value of 0.5 will give each octave half the amplitude of the one before it). The function will return NaN if persistence is exactly 1.0.
	/// @param position the position of the noise value to return.
	template <typename Iterator, typename Vector>
	typename Vector::value_type
	fractal_feedback(Iterator begin,
	                 Iterator end,
	                 typename Vector::value_type lacunarity,
	                 typename Vector::value_type persistence,
	                 const Vector& position,
	                 typename Vector::value_type precision = 0)
	{
		detail::assert_vector<Vector>();
		
		typename Vector::value_type sum = 0, per = 1, lac = 1, prev = 1;
		for (Iterator i = begin; i != end && per > precision; i++, per *= persistence, lac *= lacunarity)
		{
			typename Vector::value_type current = (*i)(position * lac) * per;
			sum += current * prev;
			prev = current;
		}
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
	
	/// Calculates the amount of octaves needed to generate noise to within a 
	/// certain margin of precision. Any octaves beyond this will not result
	/// in a change in noise values beyond that margin.
	///
	/// @param persistence the persistence value of the fractal noise. Must be between 0.0 and 1.0 (exclusive)
	/// @param precision the precision needed. A lower value will result in more octaves
	template <typename genType>
	int fractal_octaves(genType persistence, genType precision)
	{
		return (int)std::ceil(std::log(precision) / std::log(persistence));
	}
}
#endif
