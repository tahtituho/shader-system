# Tähtituho shader-system
Attempt to create simple shader demo system with support for rocket and png textures. 

# Requirements
glfw, rocket, jsoncpp, bass, glew, lodepng and glText are included in project.

- glfw version 3.2.1 https://www.glfw.org/
- rocket https://github.com/emoon/rocket
- jsoncpp version 0.10.7 https://github.com/open-source-parsers/jsoncpp
- bass version 2.4.13.8 http://www.un4seen.com/bass.html
- glew version 2.1.0 http://glew.sourceforge.net/
- lodepng https://lodev.org/lodepng/
- glText https://github.com/vallentin/glText
- 

C++ compiler is needed, and on linux g++ can found easily from package management, but on windows mingw works well
- http://www.mingw.org/
 
Make sure that include and library folders in "tasks.json" are correct for you configuration. Check "c_cpp_configuration.json" that folder are correct as well, to ensure that intellisense is working correctly.

At least these packages are needed for MinGW from Basic Setup
- mingw32-base-bin
- mingw32-gcc-g++-bin
- msys-base-bin
- additionally:
 - mingw32-libmingwex-dev
 - mingw32-libmingwex-dll

# Notes on compiling
### Windows/Linux
Create output folder for executable(ie. bin/debug) it cannot create folders by itself.

### Windows
Make sure that mingw is available, easy way to handle this is to add mingw folder (e.g. "C:\MinGW\bin") to PATH variable
Copy glfw3.dll, glew32.dll and bass.dll into output folder.

### Visual Studio Code
Visual Studio Code suits greatly to compile this, https://code.visualstudio.com/ just make sure that paths are correct for your setup in "tasks.json". Paths for following folders needs to be set to comply your setup:
- glew lib 
- glew include
- bass lib
- bass include

# executing
When demo system is executed without parameters, it defaults configuration file to "configuration.json".
If "configuration.json" is non existing, system defaults to default values, which are defined in Configuration class constructor.
Be sure that shaders, music and textures defined in "configuration.json" exist.

# usage
1. start rocket client
2. start "shader-system.exe"
Tracks defined in "configuration.json" will appear on rocket. Open fragment shader defined in "configuration.json" and verify that uniform variables corresponding to rocket tracks are present. When changes have been made to fragment shader press "r" to reload shader. 