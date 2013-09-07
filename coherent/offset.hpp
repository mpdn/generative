//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_OFFSET_HPP
#define COHERENT_OFFSET_HPP

#include <algorithm>
#include <functional>
#include <random>
#include <glm/glm.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <coherent/bind_left.hpp>

#ifndef COHERENT_IGNORE_BOOST_RESULT_OF
#ifndef BOOST_RESULT_OF_USE_DECLTYPE
#error libcoherent requires BOOST_RESULT_OF_USE_DECLTYPE to be defined in order to compile (this check can be ignored by defining COHERENT_IGNORE_BOOST_RESULT_OF)
#endif
#endif

namespace coherent
{
	namespace
	{
		template <typename Iterator>
		struct iterator_vector
		{
			typedef typename std::iterator_traits<Iterator>::value_type type;
			static_assert(detail::is_vector<type>::value, "Must be an iterator of vectors");
		};
		
		template <typename Function, typename Vector>
		auto apply_offset(Function function, Vector offset, Vector position)
		-> decltype(function(offset + position))
		{
			return function(offset + position);
		}
	}
	
	/// Fills a vector with random values
	///
	/// @tparam Vector the Vector type of the output matrix
	/// @tparam PRNG the type of the random number generator
	/// @param output the matrix to which the random values will be written to
	/// @param prng the random number generator used to generate the random values
	/// @param magnitude the maxmimum absolute value of each value
	template <typename Vector, class PRNG>
	Vector offset(PRNG& prng, typename Vector::value_type magnitude)
	{
		detail::assert_vector<Vector>();
		Vector output;
		auto gen = std::bind(std::uniform_real_distribution<typename Vector::value_type>(-magnitude, magnitude), std::ref(prng));
		for (int i = 0; i < output.length(); i++)
			output[i] = gen();
		return output;
	}
	
	/// Fills a sequence of vectors with random values
	///
	/// @tparam Iterator an iterator to vectors
	/// @tparam PRNG the type of the random number generator
	/// @param begin the iterator to the beginning of the sequence of vectors to fill with random values.
	/// @param end the iterator to the end of the sequence of vectors to fill with random values.
	/// @param prng the random number generator used to generate the random values
	/// @param magnitude the maxmimum absolute value of each value
	template <typename Iterator, class PRNG>
	void offsets(Iterator begin, Iterator end, PRNG& prng, typename std::iterator_traits<Iterator>::value_type::value_type magnitude)
	{
		std::generate(begin, end, std::bind(offset<PRNG, iterator_vector<Iterator>::type>, prng, magnitude));
	}
	
	/// Creates an iterator which, given an iterator of vectors and a
	/// a function taking said vector, applies the vector as offset for the
	/// function. This can be used to effeciently create multiple noise
	/// functions from a single noise function.
	///
	/// The object returned when dereferencing the iterator created by this
	/// function is a function object which takes a vector as parameter, adds it
	/// to the vector from the iterator, and calls the function with it.
	///
	/// @param iterator an iterator to vectors which will act as offsets
	/// @param function an function that accepts a vector as pointed to by iterator
	template <typename Iterator, typename Function>
	auto make_offset_iterator(Iterator iterator, Function function)
	-> decltype(boost::make_transform_iterator(iterator,
			detail::bind_left(
				detail::bind_left<decltype(apply_offset<Function, typename iterator_vector<Iterator>::type>),Function>,
				apply_offset<Function, typename iterator_vector<Iterator>::type>,
				function)))
	{
		return boost::make_transform_iterator(
			iterator,
			detail::bind_left(
				detail::bind_left<decltype(apply_offset<Function, typename iterator_vector<Iterator>::type>), Function>,
				apply_offset<Function, typename iterator_vector<Iterator>::type>,
				function));
	}
}

#endif
