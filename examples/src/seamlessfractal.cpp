#include <array>
#include <random>
#include <glm/glm.hpp>
#include <generative/generative.hpp>
#include "examples.hpp"
#include "TGAImage.hpp"

void generative::examples::seamlessfractal(generative::examples::TGAImage& image)
{	
	using namespace std::placeholders;
	
	const int seed = 42;
	const float frequency = 5.0f;
	const float lacunarity = 2.0f;
	const float persistence = 0.5f;
	
	std::minstd_rand prng(seed);
	std::array<glm::vec4, 8> offsets;

	generative::offsets(offsets.begin(), offsets.end(), prng, 1000.0f);

	auto begin = generative::make_offset_iterator(offsets.begin(), generative::Simplex()),
		 end   = generative::make_offset_iterator(offsets.end(), generative::Simplex());
	
	for (int y = 0; y < image.height(); y++)
		for (int x = 0; x < image.width(); x++)
		{
			glm::vec4 point = generative::seamless(1.0f, glm::vec2((float)x / image.width(), (float)y / image.height()));
			float value = generative::fractal(begin, end, lacunarity, persistence, point * frequency) * 0xFF;
			image.set(x,y, value, value, value);
		}
}
