#include <array>
#include <random>
#include <glm/glm.hpp>
#include <coherent/coherent.hpp>
#include "examples.hpp"
#include "TGAImage.hpp"

void coherent::examples::fadingfractal(coherent::examples::TGAImage& image)
{
	using namespace std::placeholders;
	
	const int seed = 42;
	const float frequency = 5.0f;
	const float lacunarity = 2.0f;
	const float persistence = 0.5f;
	
	std::minstd_rand prng(seed);
	coherent::Permutation perm(prng);
	std::array<glm::vec2, 8> offsets;

	coherent::offsets(offsets.begin(), offsets.end(), prng, 1000.0f);

	auto simplex = std::bind(coherent::Simplex(), std::cref(perm), _1);

	auto begin = coherent::make_offset_iterator(offsets.begin(), simplex),
	     end   = coherent::make_offset_iterator(offsets.end(), simplex);
	

	for (int y = 0; y < image.height(); y++)
		for (int x = 0; x < image.width(); x++)
		{
			float rx = (float)x / image.width(), ry = (float)y / image.height();
			float value = coherent::fractal(begin,
					                        end,
					                        lacunarity,
					                        persistence * ry,
					                        glm::vec2(rx,ry) * frequency) * 0xFF;
			
			image.set(x,y, value, value, value);
		}
}
