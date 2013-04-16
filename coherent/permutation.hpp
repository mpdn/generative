//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

#ifndef COHERENT_PERMUTATION_HPP
#define COHERENT_PERMUTATION_HPP

#include <algorithm>

namespace coherent
{
	/// A permutation of the values under 256. Used in certain noise functions.
	class Permutation
	{
		private:
		const static unsigned int size = 256;
		unsigned char perm[size * 2];
		
		public:
		//
		/// Creates a permutation using a random number generator.
		template<class RNG>
		explicit Permutation(RNG& prng)
		{
			for (unsigned int i = 0; i < size; i++)
				perm[i] = i;
				
			std::shuffle(perm, perm + size, prng);
			std::copy(perm, perm + size, perm + size);
		}
		
		//
		//
		template<typename Iterator>
		explicit Permutation(Iterator begin, Iterator end)
		{
#ifdef NDEBUG
			assert(std::distance(begin, end) == size);
#endif
			std::copy(begin, end, perm);
#ifdef NDEBUG
			std::sort(perm, perm + size);
			assert(std::distance(perm, std::unique(perm, perm + size)) == size);
			std::copy(begin, end, perm);
#endif
		}
		
		
		/// Returns the value at position 'i'. Allowed range is 0 <= i < 512.
		/// The last 256 values are the a repetition of the first 256.
		unsigned char operator[](unsigned int i) const
		{
			return perm[i];
		}
	};
}

#endif
