cmake_minimum_required(VERSION 3.0.0)
project(ShaderSystem VERSION 1.5.0)
configure_file(ShaderSystemConfig.h.in ShaderSystemConfig.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_VULKAN_STATIC OFF CACHE BOOL "" FORCE)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(LINUX)
    cmake_policy (SET CMP0072 OLD)
endif(LINUX)

add_subdirectory(libs/glfw-3.3.7)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLEW_INCLUDE_DIRS libs/glew-2.2.0/include)
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
)
target_include_directories(${PROJECT_NAME} PUBLIC build include libs libs/rocket libs/json libs/glfw-3.3.3/include ${GLEW_INCLUDE_DIRS} ${BASS_INCLUDE})
target_link_directories(${PROJECT_NAME} PUBLIC libs ${BASS_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows" "-fpermissive" "-static-libstdc++" "-D_WIN32_WINNT=0x501")
else(WIN32)
    if(LINUX)
        target_link_options(${PROJECT_NAME} PRIVATE "-fpermissive" "-static-libstdc++")
    endif(LINUX)
endif(WIN32)



