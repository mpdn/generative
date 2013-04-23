#ifndef COHERENT_EXAMPLES_FRACTAL_HPP
#define COHERENT_EXAMPLES_FRACTAL_HPP

#include "example.hpp"

namespace coherent
{
	namespace examples
	{
		class Fractal : public Example
		{
			int seed;
			float frequency, lacunarity, persistence;
			
			public:
			std::string description();
			void options(boost::program_options::options_description& description);
			void draw(TGAImage& image);
		};
	}
}

#endif
