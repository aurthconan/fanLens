# SDLFilm requires SDL2
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 sdl2)
IF(SDL2_FOUND)
    SET(fanLens_SOURCE_FILES ${fanLens_SOURCE_FILES}
                ${CMAKE_CURRENT_LIST_DIR}/SDLFilm.cpp )
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
    LINK_LIBRARIES(${SDL2_LIBRARIES})
ENDIF()

# FreeImageFilm requires FreeImage
# can not search for freeimage, or I just did it the wrong way
# PKG_SEARCH_MODULE(FreeImage freeimage)
IF(FreeImage_FOUND)
    SET(fanLens_SOURCE_FILES ${fanLens_SOURCE_FILES}
                ${CMAKE_CURRENT_LIST_DIR}/FreeImageFilm.cpp )
    INCLUDE_DIRECTORIES(${FreeImage_INCLUDE_DIRS})
    LINK_LIBRARIES(${FreeImage_LIBRARIES})
ELSE()
    IF(ENABLE_FREEIMAGE_SUPPORT)
        SET(fanLens_SOURCE_FILES ${fanLens_SOURCE_FILES}
                    ${CMAKE_CURRENT_LIST_DIR}/FreeImageFilm.cpp )
        LINK_LIBRARIES("freeimage")
    ENDIF()
ENDIF()
