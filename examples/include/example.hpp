#ifndef COHERENT_EXAMPLE_HPP
#define COHERENT_EXAMPLE_HPP

#ifndef COHERENT_EXAMPLES_DEFAULT_WIDTH
#define COHERENT_EXAMPLES_DEFAULT_WIDTH 512
#endif

#ifndef COHERENT_EXAMPLES_DEFAULT_HEIGHT
#define COHERENT_EXAMPLES_DEFAULT_HEIGHT 512
#endif

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>

namespace coherent
{
	namespace examples
	{
		class TGAImage
		{
			private:
			int _width, _height;
			std::vector<unsigned char> data;
			
			public:
			inline int width()  const { return _width; }
			inline int height() const { return _height; }
			
			inline void set(int x, int y, unsigned char r, unsigned char g, unsigned char b)
			{
				assert(x >= 0 && x < width());
				assert(y >= 0 && y < height());
			
				int offset = ((y * width()) + x) * 3;
				data[offset + 0] = b;
				data[offset + 1] = g;
				data[offset + 2] = b;
			}
			
			inline TGAImage(int __width, int __height)
				: _width(__width), _height(__height), data(__width * __height * 3, 0)
			{ }
			
			inline void write(std::ostream& out)
			{
				unsigned char header[18] = {0};
				header[ 2] = 2;
				header[12] =  width()        & 0xFF;
				header[13] = (width()  >> 8) & 0xFF;
				header[14] =  height()       & 0xFF;
				header[15] = (height() >> 8) & 0xFF;
				header[16] = 24;
				out.write((const char*)header, sizeof(header));
				out.write((const char*)data.data(), data.size());
			}
		};
		
		class Example
		{
			protected:
			virtual void draw(TGAImage& image) = 0;
			virtual void options(boost::program_options::options_description& description)
			{ }
			virtual std::string description() = 0;
			
			public:
			inline int execute(int argc, char **argv)
			{
				namespace po = boost::program_options;
				
				int width, height;
				std::string filename;
				
				po::options_description desc("Options");
				desc.add_options()
					("help",     "Display help")
					("width,w",  po::value<int>(&width) ->default_value(COHERENT_EXAMPLES_DEFAULT_WIDTH))
					("height,h", po::value<int>(&height)->default_value(COHERENT_EXAMPLES_DEFAULT_HEIGHT))
					("output,o", po::value<std::string>(&filename)->default_value(std::string(argv[0]) + ".tga"));
				options(desc);
				
				po::variables_map vm;
				po::store(po::parse_command_line(argc, argv, desc), vm);
				po::notify(vm);
				
				if (vm.count("help"))
				{
					std::cout << description() << std::endl << std::endl << desc << std::endl;
					return 1;
				}
				
				TGAImage image(width, height);
				draw(image);
				std::ofstream output(filename.c_str(), std::ofstream::out | std::ofstream::binary);
				image.write(output);
				output.close();
				return 0;
			}
		};
	}
}

#endif
