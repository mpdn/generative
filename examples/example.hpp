#ifndef COHERENT_EXAMPLES_HPP
#define COHERENT_EXAMPLES_HPP

#ifndef COHERENT_EXAMPLES_DEFAULT_WIDTH
#define COHERENT_EXAMPLES_DEFAULT_WIDTH 512
#endif

#ifndef COHERENT_EXAMPLES_DEFAULT_HEIGHT
#define COHERENT_EXAMPLES_DEFAULT_HEIGHT 512
#endif

#include <iostream>
#include <string>
#include <map>
#include <png++/png.hpp>

namespace coherent
{
	namespace examples
	{
		class Example
		{
			public:
			class ParameterSyntaxException : std::exception
			{
				std::string msg;
		
				public:
				inline ParameterSyntaxException(std::string _msg)
					: msg(_msg)
				{ }
		
				inline const char* what()
				{
					return msg.c_str();
				}
			};

			private:
			std::map<std::string, std::string> parameters;
	
			inline void build_parameters(int argc, char **argv)
			{
				std::string name;
				for (int i = 0; i < argc - 1; i++)
				{
					if (argv[i + 1][0] == '-')
					{
						if (name.empty())
							name = std::string(argv[i + 1] + 1);
						else
							throw ParameterSyntaxException("Expected value, got parameter name");
					}
					else
					{
						if (name.empty())
							throw ParameterSyntaxException("Expected parameter name, got value");
						else
						{
							parameters.insert(std::make_pair(name, std::string(argv[i + 1])));
							name = std::string();
						}
					}
				}
			}
	
			protected:
			typedef png::image<png::rgb_pixel> ImageType;
			virtual void operator()(ImageType& image) = 0;
	
			template <typename T>
			inline T parameter(std::string name, T default_value)
			{
				auto it = parameters.find(name);
				if (it == parameters.end())
					return default_value;
				else
				{
					T ret;
					std::istringstream ss(it->second);
					ss >> ret;
					return ret;
				}
			}
	
			public:
			inline int execute(int argc, char **argv)
			{
				build_parameters(argc, argv);
		
				std::string name = parameter("output", std::string(argv[0]) + ".png");
				int width  = parameter("width",  COHERENT_EXAMPLES_DEFAULT_WIDTH),
					height = parameter("height", COHERENT_EXAMPLES_DEFAULT_HEIGHT);
		
				try
				{
					png::image<png::rgb_pixel> image(width, height);
					(*this)(image);
					image.write(name);
					return 0;
				}
				catch(ParameterSyntaxException& e)
				{
					std::cerr << e.what() << std::endl;
					return 1;
				}
			}
		};
	}
}

#endif
