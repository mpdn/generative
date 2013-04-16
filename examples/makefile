CXXFLAGS=-I.. -I/usr/include/eigen3 -std=c++11 -Wall -O3 -Wfatal-errors

all: images
examples: SimplexFractal.out Fractal.out
images: SimplexFractal.png Fractal.png

%.out : %.cpp $(wildcard ../coherent/*)
	$(CXX) $(CXXFLAGS) $< -o $@ -lpng

%.png: %.out
	./$< -output $@

clean:
	rm -f *.out *.png

.PRECIOUS: %.out
.PHONY: examples images clean
