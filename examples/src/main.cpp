#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include "examples.hpp"
#include "TGAImage.hpp"

int main(int argc, char **argv)
{	
	int width = 512, height = 512;
	
	void (*example)(generative::examples::TGAImage& image);
	std::map<std::string, void (*)(generative::examples::TGAImage& image)> examples;
	examples["fractal"]         = generative::examples::fractal;
	examples["fadingfractal"]   = generative::examples::fadingfractal;
	examples["ridgedfractal"]   = generative::examples::ridgedfractal;
	examples["seamlessfractal"] = generative::examples::seamlessfractal;
	
	if (argc < 3 || argc > 5)
	{
		std::cout << "generative examples" << std::endl;
		std::cout << "Usage: examples file type [width] [height]" << std::endl;
		std::cout << "Available example types:" << std::endl;
		
		for (auto i : examples)
			std::cout << '\t' << i.first << std::endl;
		return 1;
	}
	
	try
	{
		auto it = examples.find(argv[2]);
		if (it == examples.end())
			throw std::invalid_argument("Invalid example type");
		example = it->second;
		
		if (argc >= 4)
			width = std::stoi(argv[3]);
		if (argc >= 5)
			width = std::stoi(argv[4]);
		if (width < 0 || height < 0)
			throw std::out_of_range("Width and height must not be negative");
		
		std::ofstream output(argv[1]);
		generative::examples::TGAImage image(width, height);
		example(image);
		image.write(output);
	}
	catch(std::invalid_argument e)
	{
		std::cerr << "Invalid value: " << e.what() << std::endl;
		return 1;
	}
	catch(std::out_of_range e)
	{
		std::cerr << "Value out of range: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
