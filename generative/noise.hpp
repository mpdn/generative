#ifndef GENERATIVE_SIMPLEX_HPP
#define GENERATIVE_SIMPLEX_HPP

#include <glm/gtc/noise.hpp>

namespace generative
{
	template <typename Vector>
	auto simplex(Vector position)
	-> decltype(glm::simplex(position))
	{
		return glm::simplex(position) / 2 + 0.5f;
	}
	
	struct Simplex
	{
		template <typename Vector>
		auto operator() (Vector position) const
		-> decltype(simplex(position))
		{
			return simplex(position);
		}
	};
	
	template <typename Vector>
	auto perlin(Vector position)
	-> decltype(glm::perlin(position))
	{
		return glm::perlin(position) / 2 + 0.5f;
	}
	
	template <typename Vector>
	auto perlin(Vector position, Vector rep)
	-> decltype(glm::perlin(position, rep))
	{
		return glm::perlin(position, rep) / 2 + 0.5f;
	}
	
	struct Perlin
	{
		template <typename Vector>
		auto operator() (Vector position) const
		-> decltype(perlin(position))
		{
			return perlin(position);
		}
		
		template <typename Vector>
		auto operator() (Vector position, Vector rep) const
		-> decltype(perlin(position, rep))
		{
			return perlin(position, rep);
		}
	};
}

#endif
