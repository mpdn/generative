#include <array>
#include <random>
#include <Eigen/Core>
#include <coherent/coherent.hpp>

#include "example.hpp"

namespace coherent
{
	namespace examples
	{
		struct SimplexFractal : Example
		{
			inline void operator()(png::image<png::rgb_pixel>& image)
			{
				using namespace std::placeholders;
		
				int seed = parameter("seed", 42);
				float frequency = parameter("frequency", 10.0f);
				float lacunarity = parameter("lacunarity", 2.0f);
				float persistence = parameter("persistence", 0.5f);
	
				std::minstd_rand prng(seed);
				coherent::Permutation perm(prng);
				std::array<Eigen::Vector2f, 8> offsets;
	
				coherent::offsets(offsets.begin(), offsets.end(), prng, 1000.0f);
	
				auto simplex = std::bind(coherent::Simplex(), std::cref(perm), _1);
	
				auto begin = coherent::make_OffsetIterator(simplex, offsets.begin()),
					 end   = coherent::make_OffsetIterator(simplex, offsets.end());
		
				int height = image.get_height();
				int width = image.get_width();
		
				for (int y = 0; y < height; y++)
					for (int x = 0; x < width; x++)
					{
						float rx = (float)x / width, ry = (float)y / height;
						float value = (coherent::fractal(begin, end, lacunarity, persistence, Eigen::Vector2f(rx,ry) * frequency) * 0.5 + 0.5) * 0xFF;
						image[y][x] = png::rgb_pixel(value, value, value);
					}
			}
		};
	}
}

int main(int argc, char **argv)
{
	return coherent::examples::SimplexFractal().execute(argc, argv);
}
