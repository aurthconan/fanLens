MACRO(ADD_DIR current_dir)
    # add source in the current dir
    AUX_SOURCE_DIRECTORY(${current_dir} temp)
    SET(fanLens_SOURCE_FILES ${fanLens_SOURCE_FILES} ${temp})
    SET(temp "")

    # search child directory
    SUBDIR_LIST(sub_dirs plain_dirs ${current_dir})

    # add child directory with its own CMakeLists.txt
    FOREACH(subdir ${sub_dirs})
        INCLUDE(${current_dir}/${subdir}/CMakeLists.txt)
    ENDFOREACH()

    # traverse the other child directory
    # without its own CMakeLists.txt
    FOREACH(dir ${plain_dirs})
        ADD_DIR(${current_dir}/${dir})
    ENDFOREACH()
ENDMACRO()

# add all subdir CMAKE
ADD_DIR(${CMAKE_CURRENT_LIST_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR})

ADD_LIBRARY(fanLens STATIC ${fanLens_SOURCE_FILES})

IF(CMAKE_COMPILER_IS_GNUCXX)
    # gcc only compiler
    SET_TARGET_PROPERTIES(fanLens PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread -std=c++0x")
    SET_TARGET_PROPERTIES(fanLens PROPERTIES COMPILE_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fopenmp -DENABLE_OPENMP=1")
ELSE()
    # not really can't support, but I do not have time to port
    MESSAGE(FATAL_ERROR "not supported compiler")
ENDIF()

