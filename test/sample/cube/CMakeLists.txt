INCLUDE_DIRECTORIES(${fanLens_SOURCE_DIR}/src)

LINK_LIBRARIES(fanLens)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 sdl2)
IF(SDL2_FOUND)
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
    LINK_LIBRARIES(${SDL2_LIBRARIES})
ELSE()
    MESSAGE(FATAL_ERROR "SDL2 lib not found")
ENDIF()

ADD_EXECUTABLE(cube main.cpp)

# add fanLens flags
IF(CMAKE_COMPILER_IS_GNUCXX)
    # gcc only compiler
    GET_TARGET_PROPERTY(fanLensFlags fanLens COMPILE_FLAGS)
    GET_TARGET_PROPERTY(fanLensFlagsRelease fanLens COMPILE_FLAGS_RELEASE)
    SET_TARGET_PROPERTIES(cube PROPERTIES COMPILE_FLAGS ${fanLensFlags})
    SET_TARGET_PROPERTIES(cube PROPERTIES COMPILE_FLAGS_RELEASE ${fanLensFlagsRelease})
ELSE()
    # not really can't support, but I do not have time to port
    MESSAGE(FATAL_ERROR "not supported compiler")
ENDIF()

