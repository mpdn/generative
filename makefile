CXXFLAGS=-I. -Iglm -std=c++11 -Wall -Wfatal-errors -DBOOST_RESULT_OF_USE_DECLTYPE
EXAMPLES=fadingfractal fractal ridgedfractal seamlessfractal

all: tgas

tga:
	mkdir -p tga

examples-bin: $(wildcard examples/*) $(wildcard generative/*)
	$(CXX) $(CXXFLAGS) -o examples-bin $(wildcard examples/*.cpp)

tga/%.tga: examples-bin |tga
	./examples-bin $@ $(notdir $(basename $@))

tgas: $(addprefix tga/,$(addsuffix .tga, $(EXAMPLES)))

clean:
	rm -rf tga
	rm -f examples-bin

.PHONY: examples/bins examples/tgas
