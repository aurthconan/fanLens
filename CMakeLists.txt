CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(fanLens)

# search for subdirectory
MACRO(SUBDIR_LIST result current_dir)
    # find all file under current_dir that is not leading with . (hidden file)
    FILE(GLOB children RELATIVE ${current_dir} ${current_dir}/[^.]*)
    SET(dirlist "")
    FOREACH(child ${children})
        # child is dir and child contains CMakeLists.txt
        IF(IS_DIRECTORY ${current_dir}/${child}
                AND EXISTS ${current_dir}/${child}/CMakeLists.txt )
            SET(dirlist ${dirlist} ${child})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# add all subdir
MACRO(ADD_ALL_SUBDIR current_dir)
    SUBDIR_LIST(sub_dirs ${current_dir})
    FOREACH(subdir ${sub_dirs})
        ADD_SUBDIRECTORY(${subdir})
    ENDFOREACH()
ENDMACRO()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# add all subdir in current dir as cmake subdirectory
ADD_ALL_SUBDIR(${fanLens_SOURCE_DIR})

