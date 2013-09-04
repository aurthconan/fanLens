SDL_DIR =
SDL_LIB =
ASSIMP_DIR =
ASSIMP_LIB =
SRC_DIR = ../../../src/

#CXX = clang++

CPPFLAGS += -I $(SRC_DIR) -L /usr/lib/ -L $(SDL_LIB) \
			-I $(SDL_DIR) -L $(ASSIMP_LIB) -I $(ASSIMP_DIR) 

CXXFLAGS += -g -Wall -Wextra -pthread -std=c++0x

all: model_viewer

clean :
	rm -f model_viewer
	find $(SRC_DIR) ./ -type f -name "*.o" -o -name "*.d" | xargs rm


src_files:=$(shell find $(SRC_DIR) ./ -type f -name "*.cpp")

# generate dependency
%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $^

include $(src_files:.cpp=.d)

%.o: %.cpp %.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -lpthread $< -o $@ -lfreeimage -lSDL2-2.0 -lassimp

model_viewer : $(patsubst %.cpp,%.o,$(src_files))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -lSDL2-2.0 -lassimp -ldl -o $@ -lfreeimage

