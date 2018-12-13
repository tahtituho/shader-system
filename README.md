# tähtituho shader-system
attempt to create simple shader demo system with support for rocket. 

# requirements
when running on ubuntu, freeglut, glew and jsoncpp are available from package management
on windows these have to be installed from following web sites
- freeGlut http://freeglut.sourceforge.net/
- glew http://glew.sourceforge.net/
- jsoncpp https://github.com/open-source-parsers/jsoncpp
- bass http://www.un4seen.com/bass.html
- rocket https://github.com/rocket/rocket

c++ compiler is needed, and on linux g++ can found easily from package management 

but on windows mingw works well
- http://www.mingw.org/
 
make sure that include and library folders in tasks.json are correct for you configuration.
  
# notes on compiling
windows/linux
create output folder for executable(ie. bin/debug) it cannot create folders by itself.

on windows copy freeglut.dll, glew32.dll and bass.dll into output folder.

# executing
when demo system is executed without parameters, it defaults configuration file to "configuration.json".
if configuration file is non existing, system default to default values, which are defined in Configuration class constructor
be sure that shaders and music defined in configuration json exist.

# usage
first start rocket client and after that shader-system.exe. tracks defined in configuration json will appear on rocket. open fragment shader defined in configuration json and verify that uniform variables corresponding to rocket tracks are present. when changes have been made to fragment shader press r to reload shader. if syntax errors are present some kind of notice will be visible in console. otherwise changes are applied.
