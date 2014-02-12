CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(fanLens)

# search for subdirectory
MACRO(SUBDIR_LIST dir_contains_cmakelists
                  dir_not_contains_cmakelists
                  current_dir)
    # find all file under current_dir that is not leading with . (hidden file)
    FILE(GLOB children RELATIVE ${current_dir} ${current_dir}/[^.]*)
    SET(dir_with_cmake "")
    SET(dir_without_cmake "")
    FOREACH(child ${children})
        # child is dir
        IF(IS_DIRECTORY ${current_dir}/${child})
            # contains CMakeLists.txt
            IF(EXISTS ${current_dir}/${child}/CMakeLists.txt)
                SET(dir_with_cmake ${dir_with_cmake} ${child})
            ELSE()
                SET(dir_without_cmake ${dir_without_cmake} ${child})
            ENDIF()
        ENDIF()
    ENDFOREACH()
    SET(${dir_contains_cmakelists} ${dir_with_cmake})
    SET(${dir_not_contains_cmakelists} ${dir_without_cmake})
ENDMACRO()

# add all subdir
MACRO(ADD_ALL_SUBDIR dir_list)
    FOREACH(subdir ${dir_list})
        ADD_SUBDIRECTORY(${subdir})
    ENDFOREACH()
ENDMACRO()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# add all topmost child dir that contains a CMakeLists.txt
MACRO(ADD_TOP_MOST_CHILD_DIR current_dir)
    # search child directory
    SUBDIR_LIST(sub_dirs plain_dirs ${current_dir})

    # add child directory with its own CMakeLists.txt
    FOREACH(subdir ${sub_dirs})
        ADD_SUBDIRECTORY(${current_dir}/${subdir})
    ENDFOREACH()

    # traverse the other child directory
    # without its own CMakeLists.txt
    FOREACH(dir ${plain_dirs})
        ADD_TOP_MOST_CHILD_DIR(${current_dir}/${dir})
    ENDFOREACH()
ENDMACRO()

ADD_TOP_MOST_CHILD_DIR(${CMAKE_CURRENT_LIST_DIR})

