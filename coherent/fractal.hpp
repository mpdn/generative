//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_FRACTAL_HPP
#define COHERENT_FRACTAL_HPP

#include <cassert>
#include <Eigen/Core>

namespace coherent
{
	/// Calculates the sum of several noise functions (called octaves) with
	/// rising frequency and decreasing amplitude.
	/// 
	/// 'begin' and 'end' corresponds to a list of noise functions.
	///
	/// 'lacunarity' is the proportional change in frequency (a value of 2 will
	/// give each octave double the frequency of the one before it).
	///
	/// 'persistence' is the the proportional change in amplitude (a value of
	/// 0.5 will give each octave half the amplitude of the one before it).
	/// The function will return NaN if this value is exactly 1.0.
	///
	/// 'position' is the position of the noise value to return.
	///
	/// The range of the return type will be the same as the noise functions
	/// (usually [0,1]).
	template <typename Iterator, typename Derived>
	typename Derived::Scalar fractal(Iterator begin,
	                                 Iterator end,
	                                 typename Derived::Scalar lacunarity,
	                                 typename Derived::Scalar persistence,
	                                 const Eigen::MatrixBase<Derived>& position)
	{
		EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived);
		
		typename Derived::Scalar sum = 0, per = 1, lac = 1;
		for (Iterator i = begin; i != end; i++, per *= persistence, lac *= lacunarity)
			sum += (*i)(position * lac) * per;
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
	
	/// Calculates the sum of several noise functions (called octaves) with
	/// rising frequency and decreasing amplitude, multiplied by the previous
	/// octave.
	///
	/// 'begin' and 'end' corresponds to a list of noise functions.
	///
	/// 'lacunarity' is the proportional change in frequency (a value of 2 will
	/// give each octave double the frequency of the one before it).
	///
	/// 'persistence' is the the proportional change in amplitude (a value of
	/// 0.5 will give each octave half the amplitude of the one before it).
	/// The function will return NaN if this value is exactly 1.0.
	///
	/// 'position' is the position of the noise value to return.
	///
	/// The range of the return type will be the same as the noise functions
	/// (usually [0,1]).
	template <typename Iterator, typename Derived>
	typename Derived::Scalar fractal_feedback(Iterator begin,
	                                          Iterator end,
	                                          typename Derived::Scalar lacunarity,
	                                          typename Derived::Scalar persistence,
	                                          const Eigen::MatrixBase<Derived>& position)
	{
		EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived);
		
		typename Derived::Scalar sum = 0, per = 1, lac = 1, prev = 1;
		for (Iterator i = begin; i != end; i++, per *= persistence, lac *= lacunarity)
		{
			typename Derived::Scalar current = (*i)(position * lac) * per;
			sum += current * prev;
			prev = current;
		}
		
		//Apply scaling factor
		return sum * (persistence - 1) / (per - 1);
	}
}
#endif
