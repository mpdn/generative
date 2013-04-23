#ifndef COHERENT_EXAMPLES_FADINGFRACTAL_HPP
#define COHERENT_EXAMPLES_FADINGFRACTAL_HPP

#include "example.hpp"

namespace coherent
{
	namespace examples
	{
		class FadingFractal : public Example
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
