#include <glm/glm.hpp>
#include <generative/generative.hpp>
#include "examples.hpp"
#include "TGAImage.hpp"

void generative::examples::seamlessfractal(generative::examples::TGAImage& image)
{
	const float frequency = 5.0f;
	const float lacunarity = 2.0f;
	const float persistence = 0.5f;
	const unsigned int octaves = 8;
	
	for (int y = 0; y < image.height(); y++)
		for (int x = 0; x < image.width(); x++)
		{
			glm::vec4 point = generative::seamless(1.0f, glm::vec2((float)x / image.width(), (float)y / image.height()));
			float value = generative::fractal(
				generative::Simplex(),
				octaves,
				lacunarity,
				persistence,
				point * frequency) * 0xFF;
			image.set(x,y, value, value, value);
		}
}
