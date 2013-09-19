/// @file
/// Simplex noise functions. An effecient gradient noise function.
///
/// These functions are heavily based on simplexnoise1234.h and simplexnoise1234.cpp
/// by Stefan Gustavson: webstaff.itn.liu.se/~stegu/aqsis/aqsis-newnoise/

#ifndef COHERENT_SIMPLEX_HPP
#define COHERENT_SIMPLEX_HPP

#include <cmath>
#include <glm/glm.hpp>
#include <generative/permutation.hpp>

namespace generative
{
	namespace
	{
		template <typename genType>
		genType grad1(int hash, genType x)
		{
			int h = hash & 15;
			genType grad = 1.0f + (h & 7);   // Gradient value 1.0, 2.0, ..., 8.0
			if (h&8) grad = -grad;          // Set a random sign for the gradient
			return ( grad * x );            // Multiply the gradient with the distance
		}
		
		template <typename genType>
		genType grad2(int hash, genType x, genType y)
		{
			int h = hash & 7;        // Convert low 3 bits of hash code
			genType u = h<4 ? x : y;  // into 8 simple gradient directions,
			genType v = h<4 ? y : x;  // and compute the dot product with (x,y).
			return ((h&1)? -u : u) + ((h&2)? -2.0f*v : 2.0f*v);
		}
		
		template <typename genType>
		genType grad3(int hash, genType x, genType y , genType z ) {
			int h = hash & 15;     // Convert low 4 bits of hash code into 12 simple
			genType u = h<8 ? x : y; // gradient directions, and compute dot product.
			genType v = h<4 ? y : h==12||h==14 ? x : z; // Fix repeats at h = 12 to 15
			return ((h&1)? -u : u) + ((h&2)? -v : v);
		}

		template <typename genType>
		genType grad4( int hash, genType x, genType y, genType z, genType t ) {
			int h = hash & 31;      // Convert low 5 bits of hash code into 32 simple
			genType u = h<24 ? x : y; // gradient directions, and compute dot product.
			genType v = h<16 ? y : z;
			genType w = h<8 ? z : t;
			return ((h&1)? -u : u) + ((h&2)? -v : v) + ((h&4)? -w : w);
		}
	}
	
	template <typename genType>
	genType simplex(const Permutation& perm, const glm::detail::tvec1<genType>& position)
	{
		int i0 = std::floor(position.x);
		int i1 = i0 + 1;
		genType x0 = position.x - i0;
		genType x1 = x0 - 1.0f;

		genType n0, n1;

		genType t0 = 1.0f - x0*x0;
		//  if(t0 < 0.0f) t0 = 0.0f; // this never happens for the 1D case
		t0 *= t0;
		n0 = t0 * t0 * grad1(perm[i0 & 0xff], x0);

		genType t1 = 1.0f - x1*x1;
		//  if(t1 < 0.0f) t1 = 0.0f; // this never happens for the 1D case
		t1 *= t1;
		n1 = t1 * t1 * grad1(perm[i1 & 0xff], x1);
		// The maximum value of this noise is 8*(3/4)^4 = 2.53125
		// A factor of 0.395 would scale to fit exactly within [-1,1], but
		// The algorithm isn't perfect, as it is assymetric. The correction will normalize the result to the interval [-1,1], but the average will be off by 3%.
		
		//Note: Changed interval to [0,1]
		return (n0 + n1 + 0.076368899f) / 4.90976220002f + 0.5;
	}
	
	template <typename genType>
	genType simplex(const Permutation& perm, const glm::detail::tvec2<genType>& position)
	{
		const genType F2 = 0.366025403f; // F2 = 0.5*(sqrt(3.0)-1.0)
		const genType G2 = 0.211324865f; // G2 = (3.0-Math.sqrt(3.0))/6.0

		genType n0, n1, n2; // Noise contributions from the three corners

		// Skew the input space to determine which simplex_traverse cell we're in
		genType s = (position.x+position.y)*F2; // Hairy factor for 2D
		genType xs = position.x + s;
		genType ys = position.y + s;
		int i = std::floor(xs);
		int j = std::floor(ys);

		genType t = (genType)(i+j)*G2;
		genType X0 = i-t; // Unskew the cell origin back to (x,y) space
		genType Y0 = j-t;
		genType x0 = position.x-X0; // The x,y distances from the cell origin
		genType y0 = position.y-Y0;

		// For the 2D case, the simplex_traverse shape is an equilateral triangle.
		// Determine which simplex_traverse we are in.
		int i1, j1; // Offsets for second (middle) corner of simplex_traverse in (i,j) coords
		if(x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
		else {i1=0; j1=1;}      // upper triangle, YX order: (0,0)->(0,1)->(1,1)

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6

		genType x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
		genType y1 = y0 - j1 + G2;
		genType x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
		genType y2 = y0 - 1.0f + 2.0f * G2;

		// Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
		int ii = i & 0xff;
		int jj = j & 0xff;

		// Calculate the contribution from the three corners
		genType t0 = 0.5f - x0*x0-y0*y0;
		if(t0 < 0.0f) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad2(perm[ii+perm[jj]], x0, y0);
		}

		genType t1 = 0.5f - x1*x1-y1*y1;
		if(t1 < 0.0f) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad2(perm[ii+i1+perm[jj+j1]], x1, y1);
		}

		genType t2 = 0.5f - x2*x2-y2*y2;
		if(t2 < 0.0f) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad2(perm[ii+1+perm[jj+1]], x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [0,1].
		
		return (n0 + n1 + n2) / 0.044217709637707734f + 0.5f ;
	}
	
	template <typename genType>
	genType simplex(const Permutation& perm, const glm::detail::tvec3<genType>& position)
	{
		// Simple skewing factors for the 3D case
		const genType F3 = 0.333333333f;
		const genType G3 = 0.166666667f;

		genType n0, n1, n2, n3; // Noise contributions from the four corners

		// Skew the input space to determine which simplex cell we're in
		genType s = (position.x+position.y+position.z)*F3; // Very nice and simple skew factor for 3D
		genType xs = position.x+s;
		genType ys = position.y+s;
		genType zs = position.z+s;
		int i = std::floor(xs);
		int j = std::floor(ys);
		int k = std::floor(zs);

		genType t = (genType)(i+j+k)*G3; 
		genType X0 = i-t; // Unskew the cell origin back to (x,y,z) space
		genType Y0 = j-t;
		genType Z0 = k-t;
		genType x0 = position.x-X0; // The x,y,z distances from the cell origin
		genType y0 = position.y-Y0;
		genType z0 = position.z-Z0;

		// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
		// Determine which simplex we are in.
		int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
		int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

		/* This code would benefit from a backport from the GLSL version! */
		if(x0>=y0) {
			if(y0>=z0)
				{ i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; } // X Y Z order
			else if(x0>=z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; } // X Z Y order
			else { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; } // Z X Y order
		}
		else { // x0<y0
			if(y0<z0) { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; } // Z Y X order
			else if(x0<z0) { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; } // Y Z X order
			else { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; } // Y X Z order
		}

		// A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
		// a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
		// a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
		// c = 1/6.

		genType x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
		genType y1 = y0 - j1 + G3;
		genType z1 = z0 - k1 + G3;
		genType x2 = x0 - i2 + 2.0f*G3; // Offsets for third corner in (x,y,z) coords
		genType y2 = y0 - j2 + 2.0f*G3;
		genType z2 = z0 - k2 + 2.0f*G3;
		genType x3 = x0 - 1.0f + 3.0f*G3; // Offsets for last corner in (x,y,z) coords
		genType y3 = y0 - 1.0f + 3.0f*G3;
		genType z3 = z0 - 1.0f + 3.0f*G3;

		// Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
		int ii = i & 0xff;
		int jj = j & 0xff;
		int kk = k & 0xff;

		// Calculate the contribution from the four corners
		genType t0 = 0.6f - x0*x0 - y0*y0 - z0*z0;
		if(t0 < 0.0f) n0 = 0.0f;
		else {
		t0 *= t0;
		n0 = t0 * t0 * grad(perm[ii+perm[jj+perm[kk]]], x0, y0, z0);
		}

		genType t1 = 0.6f - x1*x1 - y1*y1 - z1*z1;
		if(t1 < 0.0f) n1 = 0.0f;
		else {
		t1 *= t1;
		n1 = t1 * t1 * grad(perm[ii+i1+perm[jj+j1+perm[kk+k1]]], x1, y1, z1);
		}

		genType t2 = 0.6f - x2*x2 - y2*y2 - z2*z2;
		if(t2 < 0.0f) n2 = 0.0f;
		else {
		t2 *= t2;
		n2 = t2 * t2 * grad(perm[ii+i2+perm[jj+j2+perm[kk+k2]]], x2, y2, z2);
		}

		genType t3 = 0.6f - x3*x3 - y3*y3 - z3*z3;
		if(t3<0.0f) n3 = 0.0f;
		else {
		t3 *= t3;
		n3 = t3 * t3 * grad(perm[ii+1+perm[jj+1+perm[kk+1]]], x3, y3, z3);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to stay just inside [0,1]
		return 16.0f * (n0 + n1 + n2 + n3) + 0.5f; // TODO: The scale factor is preliminary!
	}
	
	template <typename genType>
	genType simplex(const Permutation& perm, const glm::detail::tvec4<genType>& position)
	{
		// A lookup table to traverse the simplex around a given point in 4D.
		// Details can be found where this table is used, in the 4D noise method.
		/* TODO: This should not be required, backport it from Bill's GLSL code! */
		const static unsigned char simplex4d[64][4] = {
			{0,1,2,3},{0,1,3,2},{0,0,0,0},{0,2,3,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,2,3,0},
			{0,2,1,3},{0,0,0,0},{0,3,1,2},{0,3,2,1},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,3,2,0},
			{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
			{1,2,0,3},{0,0,0,0},{1,3,0,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,3,0,1},{2,3,1,0},
			{1,0,2,3},{1,0,3,2},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,3,1},{0,0,0,0},{2,1,3,0},
			{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
			{2,0,1,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,0,1,2},{3,0,2,1},{0,0,0,0},{3,1,2,0},
			{2,1,0,3},{0,0,0,0},{0,0,0,0},{0,0,0,0},{3,1,0,2},{0,0,0,0},{3,2,0,1},{3,2,1,0}};
		
		// The skewing and unskewing factors are hairy again for the 4D case
		const genType F4 = 0.309016994; // F4 = (Math.sqrt(5.0)-1.0)/4.0
		const genType G4 = 0.138196601; // G4 = (5.0-Math.sqrt(5.0))/20.0
	
		genType n0, n1, n2, n3, n4; // Noise contributions from the five corners
	
		// Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
		genType s = (position.x + position.y + position.z + position.w) * F4; // Factor for 4D skewing
		genType xs = position.x + s;
		genType ys = position.y + s;
		genType zs = position.z + s;
		genType ws = position.w + s;
		int i = std::floor(xs);
		int j = std::floor(ys);
		int k = std::floor(zs);
		int l = std::floor(ws);

		genType t = (i + j + k + l) * G4; // Factor for 4D unskewing
		genType X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
		genType Y0 = j - t;
		genType Z0 = k - t;
		genType W0 = l - t;

		genType x0 = position.x - X0;  // The x,y,z,w distances from the cell origin
		genType y0 = position.y - Y0;
		genType z0 = position.z - Z0;
		genType w0 = position.w - W0;

		// For the 4D case, the simplex is a 4D shape I won't even try to describe.
		// To find out which of the 24 possible simplices we're in, we need to
		// determine the magnitude ordering of x0, y0, z0 and w0.
		// The method below is a good way of finding the ordering of x,y,z,w and
		// then find the correct traversal order for the simplex we’re in.
		// First, six pair-wise comparisons are performed between each possible pair
		// of the four coordinates, and the results are used to add up binary bits
		// for an integer index.
		int c1 = (x0 > y0) ? 32 : 0;
		int c2 = (x0 > z0) ? 16 : 0;
		int c3 = (y0 > z0) ? 8 : 0;
		int c4 = (x0 > w0) ? 4 : 0;
		int c5 = (y0 > w0) ? 2 : 0;
		int c6 = (z0 > w0) ? 1 : 0;
		int c = c1 + c2 + c3 + c4 + c5 + c6;

		int i1, j1, k1, l1; // The integer offsets for the second simplex corner
		int i2, j2, k2, l2; // The integer offsets for the third simplex corner
		int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

		// simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
		// Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
		// impossible. Only the 24 indices which have non-zero entries make any sense.
		// We use a thresholding to set the coordinates in turn from the largest magnitude.
		// The number 3 in the "simplex" array is at the position of the largest coordinate.
		i1 = simplex4d[c][0]>=3 ? 1 : 0;
		j1 = simplex4d[c][1]>=3 ? 1 : 0;
		k1 = simplex4d[c][2]>=3 ? 1 : 0;
		l1 = simplex4d[c][3]>=3 ? 1 : 0;
		// The number 2 in the "simplex" array is at the second largest coordinate.
		i2 = simplex4d[c][0]>=2 ? 1 : 0;
		j2 = simplex4d[c][1]>=2 ? 1 : 0;
		k2 = simplex4d[c][2]>=2 ? 1 : 0;
		l2 = simplex4d[c][3]>=2 ? 1 : 0;
		// The number 1 in the "simplex" array is at the second smallest coordinate.
		i3 = simplex4d[c][0]>=1 ? 1 : 0;
		j3 = simplex4d[c][1]>=1 ? 1 : 0;
		k3 = simplex4d[c][2]>=1 ? 1 : 0;
		l3 = simplex4d[c][3]>=1 ? 1 : 0;
		// The fifth corner has all coordinate offsets = 1, so no need to look that up.

		genType x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
		genType y1 = y0 - j1 + G4;
		genType z1 = z0 - k1 + G4;
		genType w1 = w0 - l1 + G4;
		genType x2 = x0 - i2 + 2.0f*G4; // Offsets for third corner in (x,y,z,w) coords
		genType y2 = y0 - j2 + 2.0f*G4;
		genType z2 = z0 - k2 + 2.0f*G4;
		genType w2 = w0 - l2 + 2.0f*G4;
		genType x3 = x0 - i3 + 3.0f*G4; // Offsets for fourth corner in (x,y,z,w) coords
		genType y3 = y0 - j3 + 3.0f*G4;
		genType z3 = z0 - k3 + 3.0f*G4;
		genType w3 = w0 - l3 + 3.0f*G4;
		genType x4 = x0 - 1.0f + 4.0f*G4; // Offsets for last corner in (x,y,z,w) coords
		genType y4 = y0 - 1.0f + 4.0f*G4;
		genType z4 = z0 - 1.0f + 4.0f*G4;
		genType w4 = w0 - 1.0f + 4.0f*G4;

		// Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
		int ii = i & 0xff;
		int jj = j & 0xff;
		int kk = k & 0xff;
		int ll = l & 0xff;

		// Calculate the contribution from the five corners
		genType t0 = 0.6f - x0*x0 - y0*y0 - z0*z0 - w0*w0;
		if(t0 < 0.0f) n0 = 0.0f;
		else {
			t0 *= t0;
			n0 = t0 * t0 * grad(perm[ii+perm[jj+perm[kk+perm[ll]]]], x0, y0, z0, w0);
		}

		genType t1 = 0.6f - x1*x1 - y1*y1 - z1*z1 - w1*w1;
		if(t1 < 0.0f) n1 = 0.0f;
		else {
			t1 *= t1;
			n1 = t1 * t1 * grad(perm[ii+i1+perm[jj+j1+perm[kk+k1+perm[ll+l1]]]], x1, y1, z1, w1);
		}

		genType t2 = 0.6f - x2*x2 - y2*y2 - z2*z2 - w2*w2;
		if(t2 < 0.0f) n2 = 0.0f;
		else {
			t2 *= t2;
			n2 = t2 * t2 * grad(perm[ii+i2+perm[jj+j2+perm[kk+k2+perm[ll+l2]]]], x2, y2, z2, w2);
		}

		genType t3 = 0.6f - x3*x3 - y3*y3 - z3*z3 - w3*w3;
		if(t3 < 0.0f) n3 = 0.0f;
		else {
			t3 *= t3;
			n3 = t3 * t3 * grad(perm[ii+i3+perm[jj+j3+perm[kk+k3+perm[ll+l3]]]], x3, y3, z3, w3);
		}

		genType t4 = 0.6f - x4*x4 - y4*y4 - z4*z4 - w4*w4;
		if(t4 < 0.0f) n4 = 0.0f;
		else {
			t4 *= t4;
			n4 = t4 * t4 * grad(perm[ii+1+perm[jj+1+perm[kk+1+perm[ll+1]]]], x4, y4, z4, w4);
		}

		// Sum up and scale the result to cover the range [-1,1]
		return 13.5f * (n0 + n1 + n2 + n3 + n4) + 0.5; // TODO: The scale factor is preliminary!
	}
	
	/// A function object that can be called like the simplex function. Use this
	/// as a function pointer to the simplex function.
	struct Simplex
	{
		template <typename genType>
		auto operator() (const Permutation& perm, const glm::detail::tvec1<genType>& position) const
			-> decltype(simplex(perm, position))
		{
			return simplex(perm, position);
		}
		
		template <typename genType>
		auto operator() (const Permutation& perm, const glm::detail::tvec2<genType>& position) const
			-> decltype(simplex(perm, position))
		{
			return simplex(perm, position);
		}
		
		template <typename genType>
		auto operator() (const Permutation& perm, const glm::detail::tvec3<genType>& position) const
			-> decltype(simplex(perm, position))
		{
			return simplex(perm, position);
		}
		
		template <typename genType>
		auto operator() (const Permutation& perm, const glm::detail::tvec4<genType>& position) const
			-> decltype(simplex(perm, position))
		{
			return simplex(perm, position);
		}
	};
}

#endif
