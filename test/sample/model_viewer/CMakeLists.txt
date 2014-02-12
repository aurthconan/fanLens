
INCLUDE_DIRECTORIES(${fanLens_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../shared_code)

LINK_LIBRARIES(fanLens shared_codes)

IF(CMAKE_COMPILER_IS_GNUCXX)
    # gcc only compiler
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -std=c++0x")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fopenmp -DENABLE_OPENMP=1")
ELSE()
    # not really can't support, but I do not have time to port
    MESSAGE(FATAL_ERROR "not supported compiler")
ENDIF()

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 sdl2)
IF(SDL2_FOUND)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
    LINK_LIBRARIES(${SDL2_LIBRARIES})
ELSE()
    MESSAGE(FATAL_ERROR "SDL2 lib not found")
ENDIF()

ADD_EXECUTABLE(model_viewer main.cpp)
