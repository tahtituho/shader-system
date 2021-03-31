cmake_minimum_required(VERSION 3.0.0)
project(ShaderSystem VERSION 1.4.0)
configure_file(ShaderSystemConfig.h.in ShaderSystemConfig.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw-3.3.3)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(GLEW_INCLUDE_DIRS libs/glew-2.2.0/include)
if(WIN32)
    set(GLEW_LIBRARIES libs/glew-2.2.0/lib/Release/Win32)
    set(BASS_LIBRARIES libs/bass24-win/c)
    set(LIBS glfw opengl32 glew32 bass ws2_32 gdi32)
else(WIN32)
    if(LINUX)
        set(BASS_LIBRARIES libs/bass24-linux/c)
        set(LIBS glfw opengl32 glew32 bass ws2_32)
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
    src/Keyboard.cpp 
    src/Helpers.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC build include libs libs/rocket libs/json libs/glfw-3.3.3/include ${GLEW_INCLUDE_DIRS} libs/bass24-win/c)
target_link_directories(${PROJECT_NAME} PUBLIC libs ${BASS_LIBRARIES} ${GLEW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows" "-fpermissive" "-static-libstdc++" "-D_WIN32_WINNT=0x501")
else(WIN32)
    if(LINUX)
        target_link_options(${PROJECT_NAME} PRIVATE "-fpermissive" "-static-libstdc++")
    endif(LINUX)
endif(WIN32)



