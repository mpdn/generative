//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_OFFSET_HPP
#define COHERENT_OFFSET_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <Eigen/Core>
#include <boost/iterator/transform_iterator.hpp>

#ifndef COHERENT_IGNORE_BOOST_RESULT_OF
#ifndef BOOST_RESULT_OF_USE_DECLTYPE
#error libcoherent requires BOOST_RESULT_OF_USE_DECLTYPE to be defined in order to compile (this check can be ignored by defining COHERENT_IGNORE_BOOST_RESULT_OF)
#endif
#endif

namespace coherent
{
	namespace
	{
		/// Provides a function object which applies an offset to a function
		/// before calling it.
		template <typename Function, typename Scalar, int dimensions>
		class OffsetPositionFunctor
		{
			typedef typename Eigen::Matrix<Scalar, dimensions, 1> Offset;
			Function function;
			Offset offset;
		
			public:
			OffsetPositionFunctor(Function _function, Offset _offset)
				: function(_function), offset(_offset)
			{ }
		
			template <typename Derived>
			auto operator()(const Eigen::MatrixBase<Derived>& position) const
				-> decltype(function(offset + position))
			{
				return function(offset + position);
			}
		};
		
		template <typename Function>
		class OffsetFunctor
		{
			Function function;
			
			public:			
			OffsetFunctor(Function _function)
				: function(_function)
			{ }
			
			template <typename Derived>
			OffsetPositionFunctor<Function, typename Derived::Scalar, Derived::RowsAtCompileTime> operator()(const Eigen::MatrixBase<Derived>& offset) const
			{
				return OffsetPositionFunctor<Function, typename Derived::Scalar, Derived::RowsAtCompileTime>(function, offset);
			}
		};
	}
	
	///
	/// Fills a vector with random values in the range [-magnitude, magnitude]
	template <typename Derived, class PRNG>
	void offset(Eigen::MatrixBase<Derived>& output, PRNG& prng, typename Derived::Scalar magnitude)
	{
		auto gen = std::bind(std::uniform_real_distribution<typename Derived::Scalar>(-magnitude, magnitude), std::ref(prng));
		for (int i = 0; i < Derived::RowsAtCompileTime; i++)
			output[i] = gen();
	}
	
	///
	/// Fills a sequence of vectors with random values in the range [-magnitude, magnitude].
	template <typename Iterator, class PRNG>
	void offsets(Iterator begin, Iterator end, PRNG& prng, typename std::iterator_traits<Iterator>::value_type::Scalar magnitude)
	{
		for (Iterator i = begin; i != end; i++)
			offset(*i, prng, magnitude);
	}
	
	///
	/// Creates an OffsetIterator (lets us use type inference)
	template <typename Iterator, typename Function>
	auto make_offset_iterator(Iterator iterator, Function function)
		-> decltype(boost::make_transform_iterator(iterator, OffsetFunctor<Function>(function)))
	{
		return boost::make_transform_iterator(iterator, OffsetFunctor<Function>(function));
	}
}

#endif
