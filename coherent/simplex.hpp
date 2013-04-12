//Copyright (c) 2013 Mike Pedersen
//See the file license.txt for copying permission.

// This file is based on simplexnoise1234.h and simplexnoise1234.cpp by
// Stefan Gustavson, originally released into public domain.

// TODO: Get 3 and 4 dimensional noise done.

#ifndef COHERENT_SIMPLEX_HPP
#define COHERENT_SIMPLEX_HPP

#include <cmath>
#include <type_traits>
#include <Eigen/Core>
#include <coherent/permutation.hpp>

namespace coherent
{
	namespace
	{
		template <typename Scalar>
		Scalar grad1(int hash, Scalar x)
		{
			int h = hash & 15;
			Scalar grad = 1.0f + (h & 7);   // Gradient value 1.0, 2.0, ..., 8.0
			if (h&8) grad = -grad;          // Set a random sign for the gradient
			return ( grad * x );            // Multiply the gradient with the distance
		}
		
		template <typename Scalar>
		Scalar grad2(int hash, Scalar x, Scalar y)
		{
			int h = hash & 7;        // Convert low 3 bits of hash code
			Scalar u = h<4 ? x : y;  // into 8 simple gradient directions,
			Scalar v = h<4 ? y : x;  // and compute the dot product with (x,y).
			return ((h&1)? -u : u) + ((h&2)? -2.0f*v : 2.0f*v);
		}
	}
	
	template <typename Scalar>
	typename std::enable_if<std::is_floating_point<Scalar>::value, Scalar>::type
		simplex(const Permutation& perm, Scalar x)
	{
		int i0 = std::floor(x);
		int i1 = i0 + 1;
		Scalar x0 = x - i0;
		Scalar x1 = x0 - 1.0f;

		Scalar n0, n1;

		Scalar t0 = 1.0f - x0*x0;
		//  if(t0 < 0.0f) t0 = 0.0f; // this never happens for the 1D case
		t0 *= t0;
		n0 = t0 * t0 * grad1(perm[i0 & 0xff], x0);

		Scalar t1 = 1.0f - x1*x1;
		//  if(t1 < 0.0f) t1 = 0.0f; // this never happens for the 1D case
		t1 *= t1;
		n1 = t1 * t1 * grad1(perm[i1 & 0xff], x1);
		// The maximum value of this noise is 8*(3/4)^4 = 2.53125
		// A factor of 0.395 would scale to fit exactly within [-1,1], but
		// The algorithm isn't perfect, as it is assymetric. The correction will normalize the result to the interval [-1,1], but the average will be off by 3%.
		return (n0 + n1 + 0.076368899f) / 2.45488110001f;
	}
	
	template <typename Scalar>
	Scalar simplex(const Permutation& perm, const Eigen::Matrix<Scalar, 1, 1>& position)
	{
		return simplex(perm, position[0]);
	}
	
	template <typename Scalar>
	Scalar simplex(const Permutation& perm, const Eigen::Matrix<Scalar, 2, 1>& position)
	{
		Scalar x = position[0], y = position[1];
	
		const Scalar F2 = 0.366025403f; // F2 = 0.5*(sqrt(3.0)-1.0)
		const Scalar G2 = 0.211324865f; // G2 = (3.0-Math.sqrt(3.0))/6.0

		Scalar n0, n1, n2; // Noise contributions from the three corners

		// Skew the input space to determine which simplex_traverse cell we're in
		Scalar s = (x+y)*F2; // Hairy factor for 2D
		Scalar xs = x + s;
		Scalar ys = y + s;
		int i = std::floor(xs);
		int j = std::floor(ys);

		Scalar t = (Scalar)(i+j)*G2;
		Scalar X0 = i-t; // Unskew the cell origin back to (x,y) space
		Scalar Y0 = j-t;
		Scalar x0 = x-X0; // The x,y distances from the cell origin
		Scalar y0 = y-Y0;

		// For the 2D case, the simplex_traverse shape is an equilateral triangle.
		// Determine which simplex_traverse we are in.
		int i1, j1; // Offsets for second (middle) corner of simplex_traverse in (i,j) coords
		if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
		else {i1=0; j1=1;}      // upper triangle, YX order: (0,0)->(0,1)->(1,1)

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6

		Scalar x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
		Scalar y1 = y0 - j1 + G2;
		Scalar x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
		Scalar y2 = y0 - 1.0f + 2.0f * G2;

		// Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
		int ii = i & 0xff;
		int jj = j & 0xff;

		// Calculate the contribution from the three corners
		Scalar t0 = 0.5f - x0*x0-y0*y0;
		if(t0 < 0.0f) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad2(perm[ii+perm[jj]], x0, y0);
		}

		Scalar t1 = 0.5f - x1*x1-y1*y1;
		if(t1 < 0.0f) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad2(perm[ii+i1+perm[jj+j1]], x1, y1);
		}

		Scalar t2 = 0.5f - x2*x2-y2*y2;
		if(t2 < 0.0f) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad2(perm[ii+1+perm[jj+1]], x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		
		return (n0 + n1 + n2) / 0.022108854818853867f;
	}
	
	template <typename Derived>
	typename std::enable_if<Derived::RowsAtCompileTime == 1, typename Derived::Scalar>::type
		simplex(const Permutation& perm, const Eigen::MatrixBase<Derived>& pos)
	{
		return simplex(perm, Eigen::Matrix<typename Derived::Scalar, 1, 1>(pos));
	}
	
	template <typename Derived>
	typename std::enable_if<Derived::RowsAtCompileTime == 2, typename Derived::Scalar>::type
		simplex(const Permutation& perm, const Eigen::MatrixBase<Derived>& pos)
	{
		return simplex(perm, Eigen::Matrix<typename Derived::Scalar, 2, 1>(pos));
	}
	
	struct Simplex
	{
		template <typename Derived>
		auto operator() (const Permutation& perm, const Eigen::MatrixBase<Derived>& pos) const
			-> decltype(simplex(perm, pos))
		{
			return simplex(perm, pos);
		}
	};
}

#endif
