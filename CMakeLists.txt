cmake_minimum_required(VERSION 3.23.1)
project(ShaderSystem VERSION 1.5.0)
configure_file(ShaderSystemConfig.h.in ShaderSystemConfig.h)
# Most probably we need to copy exe and dlls files to Debug folder
# Also assets folder and configuration.json should be copied to Debug folder
# By doing that we ensure that working directory is correct, maybe

# This commands cmake to copy all compiled stuff to build folder
# Reasoning for this is that with that we get glfw.dll next to exe
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

set(WORKING_DIRECTORY, PROJECT_SOURCE_DIR)
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(LINUX)
    cmake_policy (SET CMP0072 OLD)
endif(LINUX)

# This adds glfw folder so cmake knows where to look for(?)
add_subdirectory(libs/glfw-3.3.7)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLEW_INCLUDE libs/glew-2.2.0/include)
set(GLFW_INCLUDE libs/glfw-3.3.7/include)
if(WIN32)
    set(GLEW_LIBRARIES libs/glew-2.2.0/lib/Release/Win32)
    set(BASS_INCLUDE libs/bass24-win/c)
    set(BASS_LIBRARIES libs/bass24-win/c)
    set(LIBS glfw opengl32 glew32 bass ws2_32 gdi32)
else(WIN32)
    if(LINUX)
        set(BASS_INCLUDE libs/bass24-linux)
        set(BASS_LIBRARIES libs/bass24-linux/x64)
        set(LIBS GLEW GLU GL glfw bass)
    endif(LINUX)
endif(WIN32)

add_executable(${PROJECT_NAME} 
    main.cpp 
    libs/rocket/device.c 
    libs/rocket/track.c 
    libs/jsoncpp.cpp 
    libs/lodepng.cpp 
    src/Synchronizer.cpp 
    src/Configuration.cpp 
    src/Music.cpp 
    src/Textures.cpp 
    src/Logger.cpp 
    src/Graphics.cpp 
    src/InputDevices.cpp 
    src/Helpers.cpp
    src/Camera.cpp
    src/Vector3.cpp
    src/Framebuffer.cpp
) 

# This command includes header file directories, like rocket and glfw
target_include_directories(${PROJECT_NAME} PUBLIC build include libs libs/rocket libs/json ${GLFW_INCLUDE} ${GLEW_INCLUDE} ${BASS_INCLUDE})

# This command tells where to find lib files (why we don't have GLFW here?)
target_link_directories(${PROJECT_NAME} PUBLIC libs ${GLEW_LIBRARIES} ${BASS_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} ${LIBS})

# This copies files to build folder, which are needed to run shader system
# assets folder contains glsl source, textures, music etc
# configuration file is for configuration
# then copy precompiled glew32.dll, bass.dll (not sure about linux part here)
file(COPY assets DESTINATION .)
file(COPY configuration.json DESTINATION .)
if(WIN32)
    file(COPY libs/bass24-win/bass.dll DESTINATION .)
    file(COPY libs/glew-2.2.0/bin/Release/Win32/glew32.dll DESTINATION .)
else(WIN32)
    if(LINUX)
        file(COPY libs/bass24-linux/libbass.so DESTINATION .)
    endif(LINUX)
endif(WIN32)

# target_link_options are parameters for compiler (g++ or clang++)
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows" "-fpermissive" "-static-libstdc++" "-D_WIN32_WINNT=0x501")
else(WIN32)
    if(LINUX)
        target_link_options(${PROJECT_NAME} PRIVATE "-fpermissive" "-static-libstdc++")
    endif(LINUX)
endif(WIN32)



