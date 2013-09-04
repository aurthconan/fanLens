# google test framework folder
GTEST_DIR =

# sdl 2.0 include folder
SDL_DIR =
SDL_LIB =

SRC_DIR = ../../src/

OUT_DIR = .

#CXX = clang++

CPPFLAGS += -isystem $(GTEST_DIR)/include -I $(SRC_DIR) -L /usr/lib/ -I $(SDL_DIR) -L $(SDL_LIB)

CXXFLAGS += -g -Wall -Wextra -pthread -std=c++0x

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all : fanLens_test run_test

run_test: fanLens_test
	./fanLens_test

clean :
	rm -f gtest.a gtest_main.a fanLens_test
	find $(SRC_DIR) ./ -type f -name "*.o" -o -name "*.d" | xargs rm

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

test_src_files:=$(shell find $(SRC_DIR) ./ -type f -name "*.cpp")

# generate dependency
%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $^

include $(test_src_files:.cpp=.d)

%.o: %.cpp %.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -lpthread $< -o $@ -lfreeimage

fanLens_test : $(patsubst %.cpp,%.o,$(test_src_files)) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -ldl -lSDL2-2.0 -o $@ -lfreeimage

.PHONY: clean run_test
