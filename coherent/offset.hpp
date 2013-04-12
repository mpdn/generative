//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_OFFSET_HPP
#define COHERENT_OFFSET_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <Eigen/Core>

namespace coherent
{
	namespace
	{
		/// Provides a function object which applies an offset to a function
		/// before calling it.
		template <typename Function, typename Scalar, int dimensions>
		class OffsetFunctor
		{
			typedef typename Eigen::Matrix<Scalar, dimensions, 1> Offset;
			Function function;
			Offset offset;
		
			public:
			OffsetFunctor(Function _function, Offset _offset)
				: function(_function), offset(_offset)
			{ }
		
			template <typename Derived>
			auto operator()(const Eigen::MatrixBase<Derived>& position) const
				-> decltype(function(offset + position))
			{
				return function(offset + position);
			}
		};
	
		///
		/// Creates an OffsetFunctor (makes it possible to use type inference).
		template <typename Function, typename Scalar, int dimensions>
		OffsetFunctor<Function, Scalar, dimensions> make_OffsetFunctor(Function function, Eigen::Matrix<Scalar, dimensions, 1> offset)
		{
			return OffsetFunctor<Function, Scalar, dimensions>(function, offset);
		}
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
	
	/// Uses an offset from a iterator along with a noise function to create
	/// offset noise. Useful for generating several similar noise function from
	/// a single noise function.
	template <typename Function, typename Iterator>
	class OffsetIterator
		: std::iterator<std::input_iterator_tag,
		                OffsetFunctor<Function,
		                              typename std::iterator_traits<Iterator>::value_type::Scalar,
		                              std::iterator_traits<Iterator>::value_type::RowsAtCompileTime>>
	                                      
	{
		using typename std::iterator<std::input_iterator_tag,
		                             OffsetFunctor<Function,
		                                           typename std::iterator_traits<Iterator>::value_type::Scalar,
		                                           std::iterator_traits<Iterator>::value_type::RowsAtCompileTime>>::value_type;
		
		Function function;
		Iterator iterator;
	
		public:
		OffsetIterator(Function _function, Iterator _iterator)
			: function(_function), iterator(_iterator)
		{ }
		
		bool operator== (const OffsetIterator<Function, Iterator>& other) const
		{
			return iterator == other.iterator;
		}
		
		bool operator!= (const OffsetIterator<Function, Iterator>& other) const
		{
			return iterator != other.iterator;
		}
		
		OffsetIterator<Function, Iterator>& operator++()
		{
			++iterator;
			return *this;
		}
		
		OffsetIterator<Function, Iterator> operator++(int)
		{
			OffsetIterator<Function, Iterator> tmp(*this);
			operator++();
			return tmp;
		}
		
		value_type operator* () const
		{
			return value_type(function, *iterator);
		}
	};
	
	///
	/// Creates an OffsetIterator (lets us use type inference)
	template <typename Function, typename Iterator>
	OffsetIterator<Function, Iterator> make_OffsetIterator(Function function, Iterator iterator)
	{
		return OffsetIterator<Function, Iterator>(function, iterator);
	}
}

#endif
