//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_PERMUTATION_HPP
#define COHERENT_PERMUTATION_HPP

#include <algorithm>
#include <cassert>

namespace generative
{
	///
	/// A permutation of the values under 256. Used in certain noise functions.
	class Permutation
	{
		private:
		const static unsigned int size = 256;
		unsigned char perm[size];
		
		public:
		/// Creates a permutation using a random number generator
		/// @param prng the random number generator to use
		template<class RNG>
		explicit Permutation(RNG& prng)
		{
			for (unsigned int i = 0; i < size; i++)
				perm[i] = i;
				
			std::shuffle(perm, perm + size, prng);
		}
		
		/// Creates a permutation from another sequence
		/// @param begin the beginning of the sequence
		/// @param end the end of the sequence
		template<typename Iterator>
		explicit Permutation(Iterator begin, Iterator end)
		{
			assert(std::distance(begin, end) == size &&
				"Distance from begin to end must qual size (256)");
			std::copy_n(begin, size, perm);
#ifndef NDEBUG
			std::sort(perm, perm + size);
			assert(std::distance(perm, std::unique(perm, perm + size)) == size &&
				"Values in begin to end must be a permutation of 0 to 256 (no duplicate values)");
			std::copy(begin, end, perm);
#endif
		}
		
		
		/// Returns the permutation value at a certain index
		/// @param index the index of the value to return
		unsigned char operator[](unsigned int index) const
		{
			return perm[index % 256];
		}
		
		/// The iterator type used when iterator over the permutation
		typedef const unsigned char* iterator;
		
		/// Returns an iterator to the beginning of the permutation
		iterator begin() const
		{
			return perm;
		}
		
		/// Returns an iterator just past the end of the permutation
		iterator end() const
		{
			return perm + size;
		}
	};
}

#endif
