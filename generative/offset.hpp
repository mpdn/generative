#ifndef GENERATIVE_OFFSET_HPP
#define GENERATIVE_OFFSET_HPP

#include <algorithm>
#include <functional>
#include <random>
#include <glm/glm.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <generative/vector_traits.hpp>

#ifndef GENERATIVE_IGNORE_BOOST_RESULT_OF
#ifndef BOOST_RESULT_OF_USE_DECLTYPE
#error generative requires BOOST_RESULT_OF_USE_DECLTYPE to be defined in order to compile (this check can be ignored by defining GENERATIVE_IGNORE_BOOST_RESULT_OF)
#endif
#endif

namespace generative
{
	namespace
	{
		/// Provides a function object which applies an offset to a function
		/// before calling it.
		template <typename Function, typename Vector>
		class OffsetPositionFunctor
		{
			Function function;
			Vector offset;

			public:
			OffsetPositionFunctor(Function _function, Vector _offset)
				: function(_function), offset(_offset)
			{ }
			
			auto operator()(const Vector& position) const
				-> decltype(function(offset + position))
			{
				return function(offset + position);
			}
		};

		/// Function object that, given a function, binds a offset to it and 
		/// returns a OffsetPositionFunctor that then applies the offset to a
		/// position.
		template <typename Function>
		class OffsetFunctor
		{
			Function function;

			public:			
			OffsetFunctor(Function _function)
				: function(_function)
			{ }

			template <typename Vector>
			OffsetPositionFunctor<Function, Vector> operator()(const Vector& offset) const
			{
				detail::assert_vector<Vector>();
				return OffsetPositionFunctor<Function, Vector>(function, offset);
			}
		};
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
		for (unsigned int i = 0; i < output.length(); i++)
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
		detail::assert_vector<typename std::iterator_traits<Iterator>::value_type>();
		std::generate(begin, end, std::bind(offset<typename std::iterator_traits<Iterator>::value_type, PRNG>, prng, magnitude));
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
		-> decltype(boost::make_transform_iterator(iterator, OffsetFunctor<Function>(function)))
	{
		return boost::make_transform_iterator(iterator, OffsetFunctor<Function>(function));
	}
}

#endif
