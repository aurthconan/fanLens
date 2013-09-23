SDL_DIR =
SDL_LIB =
SRC_DIR = ../../../src/

#CXX = clang++

CPPFLAGS += -I $(SRC_DIR) -L /usr/lib/ -I $(SDL_DIR)

CXXFLAGS += -g -Wall -Wextra -pthread -std=c++0x -O3

all: cube_example

clean :
	rm -f cube_example
	find $(SRC_DIR) ./ -type f -name "*.o" -o -name "*.d" | xargs rm


src_files:=$(shell find $(SRC_DIR) ./ -type f -name "*.cpp")

# generate dependency
%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $^

include $(src_files:.cpp=.d)

%.o: %.cpp %.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -lpthread $< -o $@ -lfreeimage

cube_example : $(patsubst %.cpp,%.o,$(src_files))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ $(SDL_LIB)/libSDL2.a -ldl -o $@ -lfreeimage

