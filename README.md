# Tähtituho shader-system
Attempt to create simple shader demo system with support for rocket and png textures. 

# Requirements
glfw, rocket, jsoncpp, bass, glew, lodepng and glText are included in project.

- glfw version 3.3.3 https://www.glfw.org/
- rocket https://github.com/emoon/rocket
- jsoncpp version 0.10.7 https://github.com/open-source-parsers/jsoncpp
- bass version 2.4.13.8 http://www.un4seen.com/bass.html
- glew version 2.2.0 http://glew.sourceforge.net/
- lodepng https://lodev.org/lodepng/
- glText https://github.com/vallentin/glText


C++ compiler is needed, and on linux g++ can found easily from package management, but on windows mingw-64 works well.
- https://www.mingw-w64.org/downloads/ choose MSYS2 (other options might be viable, but MSYS2 is known to work)

After MSYS2 is installed open MSYS2 and type

`pacman -S --needed base-devel mingw-w64-i686-toolchain` (will install 32 bit version)

Make sure visual studio code knows where to find `g++.exe`. Two options are provided here, and option number **2** is preferred.

1. Make sure that miDebuggerPath is correct in `lauch.json`. Check `c_cpp_configuration.json` that compilerPath is correct as well, to ensure that intellisense is working correctly. **NOTE** do not commit those changes to repository, as they are specific to your system.
2. add mingw bin folder to your PATH, for example `C:\development\msys64\mingw32\bin` (preferred way)

More information about compiler installation: [C/C++ for Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp)

**CMake** is needed. Download it from [https://cmake.org/](https://cmake.org/) and add it to your path.

Install **CMake** and **CMake Tools** extensions for Visual Studio Code, both can be found from Visual Studio Extensions

**Ninja** is needed, Download it from [https://ninja-build.org/](https://ninja-build.org/) and install it somewhere and add installation path to your path, so cmake can find it.

Example of `c_cpp_properties.json` is provided and you should copy `c_cpp_properties_example.json` to `.vscode` and rename it to `c_cpp_properties.json` and make changes accordinug to your setup.

# Notes on compiling
### Windows/Linux
Create output folder for executable(ie. bin/debug) as compiler cannot create folders by itself.

### Windows
Make sure that mingw is available, easy way to handle this is to add mingw folder (e.g. "C:\MinGW\bin") to PATH variable
Make sure that `.vscode/settings.json` has `"C_Cpp.intelliSenseEngine": "Tag Parser"`

### Linux
Use following command to install required libraries
`sudo apt-get install g++-multilib libglew-dev xorg-dev`

### Compiling
Use command *CMake: Configure* to create configuration. Configuration will appear in **build** folder. Use command *CMake: Build* to compile shader system. 

# Debugging
Before debugging copy following files NOTE these steps has to be done only once, not every time you debug.
1. Copy bass.dll from libs/bass-24-win/ **build** folder
1. Copy glew32.dl from libs/glew-2.2.0/bin/Release **build** folder

There is two different ways to debug
- Pressing **F5** will use debug profiles defined in `.vscode/launch.json` 
- Pressing small bug icon on lower part of Visual Studio Code, this will create it's own definition on the fly (preferred method)

# Executing
To run shader-system in release mode execute following steps. Release mode means mode where you write shader code, sync graphics with music and present your demo at party.
1. Make directory where you want to store shader-system.
1. Build relese version.
1. Copy bass.dl from libs/bass-24-win/ to newly created folder
1. Copy glew32.dl from libs/glew-2.2.0/bin/Release to newly created folder
1. Copy glfw3.dll from /build/libs/glfw-3.3.7/src to newly created folder

When demo system is executed without parameters, it defaults configuration file to "configuration.json".

Be sure that shaders, music and textures defined in "configuration.json" exist, as shader system does not give any error messages if some files are missing.

1. start rocket client
2. start "ShaderSystem.exe"
Tracks defined in "configuration.json" will appear on rocket. Open fragment shader defined in "configuration.json" and verify that uniform variables corresponding to rocket tracks are present. When changes have been made to fragment shader press "r" to reload shader. Press "s" to turn tune on or off.

# Credits
Included example song **Monplaisir - Feet Gone Wild Speed Version**: [http://ccmixter.org/files/Monplaisir/56217](http://ccmixter.org/files/Monplaisir/56217)