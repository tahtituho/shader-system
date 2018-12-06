# shader-system
attempt to create simple shader demo system with support for rocket. 

# requirements
when running on ubuntu, freeglut, glew and jsoncpp are available from package management
on windows these have to be installed from following web sites
- freeGlut http://freeglut.sourceforge.net/
- glew http://glew.sourceforge.net/
- jsoncpp https://github.com/open-source-parsers/jsoncpp
  
# notes on compiling
windows/linux
create output folder for executable(ie. bin/debug)
on windows copy freeglut.dll and glew32.dll into output folder.

# executing
when demo system is executed without parameters, it defaults configuration file to "configuration.json".
if configuration file is non existing, system default to default values, which are defined in Configuration class constructor

