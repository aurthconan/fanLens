GTEST_DIR = /home/jianghuai/source/googletest-read-only

OUT_DIR = .

CPPFLAGS += -isystem $(GTEST_DIR)/include -I ../src/

CXXFLAGS += -g -Wall -Wextra -pthread

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all : fanLens_test run_test

run_test: fanLens_test
	./fanLens_test

clean :
	rm -f gtest.a gtest_main.a *.o fanLens_test *.d

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

test_src_files:=$(shell find -name "*.cpp")

# generate dependency
%.d: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ $^

include $(test_src_files:.cpp=.d)

%.o: %.cpp %.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -lpthread $< -o $@

fanLens_test : $(patsubst %.cpp,%.o,$(test_src_files)) gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

.PHONY: clean run_test
