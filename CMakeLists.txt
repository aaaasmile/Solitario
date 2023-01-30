# Uso CMake principalmente per il target Msys2 su Windows
#$ cmake --version
#cmake version 3.25.2

cmake_minimum_required(VERSION 3.20)
project(solitario)

#Libini
#add_subdirectory(libini)
set(LIBINISRCS libini/headings.h
                libini/ini.cpp
                libini/ini.h
                libini/keys.h
                libini/list.h)
add_library(libini ${LIBINISRCS})
target_include_directories(libini PUBLIC libini/include)
target_compile_definitions(libini PRIVATE HAVE_STRCASECMP)

# Solitario program
set(SOURCES src/GameGfx/CardRegionGfx.cpp
            src/GameGfx/CardStackGfx.cpp
            src/GameGfx/SolitarioGfx.cpp
            src/MusicManager.cpp
            src/AppGfx.cpp
            src/CardGfx.cpp
            src/DeckType.cpp
            src/GameSettings.cpp
            src/GfxUtil.cpp
            src/Languages.cpp
            src/Main.cpp
            src/Fading.cpp
            src/Credits.cpp
            src/ErrorInfo.cpp
            src/MenuMgr.cpp
            src/MainOptionGfx.cpp
            src/CompGfx/cLabelLinkGfx.cpp
            src/CompGfx/cButtonGfx.cpp
            src/CompGfx/cCheckBoxGfx.cpp
            src/CompGfx/cComboGfx.cpp
            src/CompGfx/cLabelGfx.cpp)



add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_compile_definitions(${PROJECT_NAME} PRIVATE DATA_PREFIX="data/")

if(CMAKE_SYSTEM MATCHES Windows)
   message(STATUS "Target system is Windows: " ${CMAKE_SYSTEM} " MySys: " ${CMAKE_HOST_WIN32})
    if(CMAKE_HOST_WIN32)
        message(STATUS "OK MySys on Windows is a supported target")    
        target_compile_definitions(${PROJECT_NAME} PRIVATE main=SDL_main)
        target_include_directories(${PROJECT_NAME}  PRIVATE C:/msys64/ucrt64/include/SDL2)

        target_link_libraries(${PROJECT_NAME} mingw32 SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)
    endif()       
endif()

target_link_libraries(${PROJECT_NAME} libini)

