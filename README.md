# tähtituho shader-system
attempt to create simple shader demo system with support for rocket. 

# requirements
glfw, rocket and jsoncpp are included in project. other libraries must be installed from following locations:

- glfw version 3.2.1 https://www.glfw.org/
- rocket commit 901db86412a0d57600cb072c16deac9c3ebc709d https://github.com/rocket/rocket
- jsoncpp version 0.10.7 https://github.com/open-source-parsers/jsoncpp
- bass version 2.4.13.8 http://www.un4seen.com/bass.html
- glew version 2.1.0 http://glew.sourceforge.net/

all libraries are 32-bit.

c++ compiler is needed, and on linux g++ can found easily from package management 

but on windows mingw works well
- http://www.mingw.org/
 
make sure that include and library folders in tasks.json are correct for you configuration. and check c_cpp_configuration.json that folder are correct as well, to ensure that intellisense is working correctly.
  
# notes on compiling
windows/linux
create output folder for executable(ie. bin/debug) it cannot create folders by itself.

windows
make sure that mingw is available, easy way to handle this is to add mingw folder to PATH variable
copy glfw3.dll, glew32.dll and bass.dll into output folder.

# executing
when demo system is executed without parameters, it defaults configuration file to "configuration.json".
if configuration file is non existing, system default to default values, which are defined in Configuration class constructor
be sure that shaders and music defined in configuration json exist.

# usage
first start rocket client and after that shader-system.exe. tracks defined in configuration json will appear on rocket. open fragment shader defined in configuration json and verify that uniform variables corresponding to rocket tracks are present. when changes have been made to fragment shader press r to reload shader. if syntax errors are present some kind of notice will be visible in console. otherwise changes are applied.
