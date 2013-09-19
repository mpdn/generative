CXXFLAGS=-I. -Iglm -std=c++11 -Wall -Wfatal-errors -DBOOST_RESULT_OF_USE_DECLTYPE
EXAMPLES=fadingfractal fractal ridgedfractal

all: examples/tgas

examples/tga:
	mkdir -p examples/tga

examples/examples: $(wildcard examples/src/*) $(wildcard generative/*)
	$(CXX) $(CXXFLAGS) -o examples/examples $(wildcard examples/src/*.cpp)

examples/tga/%.tga: examples/examples |examples/tga
	examples/examples $@ $(notdir $(basename $@))

examples/bins: $(addprefix examples/bin/,$(EXAMPLES))

examples/tgas: $(addprefix examples/tga/,$(addsuffix .tga, $(EXAMPLES)))

.PHONY: examples/bins examples/tgas
