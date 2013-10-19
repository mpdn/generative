#include <glm/glm.hpp>
#include <generative/generative.hpp>
#include "examples.hpp"
#include "TGAImage.hpp"

void generative::examples::ridgedfractal(generative::examples::TGAImage& image)
{
	const float frequency = 5.0f;
	const float lacunarity = 2.0f;
	const float persistence = 0.5f;
	const unsigned int octaves = 8;
	
	auto simplex = [&](glm::vec2 v){ return generative::fabs(generative::simplex(v)); };
	for (int y = 0; y < image.height(); y++)
		for (int x = 0; x < image.width(); x++)
		{
			float rx = (float)x / image.width(), ry = (float)y / image.height();
			float value = generative::invert(generative::fractal_feedback(
				simplex,
				octaves,
				lacunarity,
				persistence,
				glm::vec2(rx,ry) * frequency)) * 0xFF;
			image.set(x, y, value, value, value);
		}
}
