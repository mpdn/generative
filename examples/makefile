CXXFLAGS=-I.. -I/usr/include/eigen3 -std=c++11 -Wall -O3 -Wfatal-errors -DBOOST_RESULT_OF_USE_DECLTYPE

all: images
examples: SimplexFractal.out Fractal.out RidgedMultifractal.out
images: SimplexFractal.png Fractal.png RidgedMultifractal.png

%.out : %.cpp example.hpp $(wildcard ../coherent/*)
	$(CXX) $(CXXFLAGS) $< -o $@ -lpng

%.png: %.out
	./$< -output $@

clean:
	rm -f *.out *.png

.PRECIOUS: %.out
.PHONY: examples images clean
