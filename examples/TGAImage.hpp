#ifndef COHERENT_EXAMPLES_TGAIMAGE_HPP
#define COHERENT_EXAMPLES_TGAIMAGE_HPP

#include <vector>
#include <cassert>

namespace generative
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
				data[offset + 0] = r;
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
	}
}

#endif // COHERENT_EXAMPLES_TGAIMAGE_HPP
