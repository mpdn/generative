CXXFLAGS=-I. -Iglm -std=c++11 -Wall -Wfatal-errors -DBOOST_RESULT_OF_USE_DECLTYPE
CXX=clang++
EXAMPLES=fadingfractal fractal ridgedfractal

all: examples/tgas examples/bins

examples/bin:
	mkdir -p examples/bin

examples/tga:
	mkdir -p examples/tga

examples/bin/%: examples/src/%.cpp $(wildcard examples/include/*) $(wildcard coherent/*) |examples/bin
	$(CXX) $(CXXFLAGS) -O3 -DNDEBUG -Iexamples/include -o $@ $< -lboost_program_options

examples/tga/%.tga: examples/bin/% |examples/tga
	$< -o $@

examples/bins: $(addprefix examples/bin/,$(EXAMPLES))

examples/tgas: $(addprefix examples/tga/,$(addsuffix .tga, $(EXAMPLES)))

.PHONY: examples/bins examples/tgas
