#ifndef TEXTURES_H
#define TEXTURES_H
#include "lodepng.h"
#include <string>
#include <list>
#include <vector>
#include "Configuration.h"
#include "Helpers.h"

namespace DemoSystem
{
    class Textures
    {
    public:
        struct Texture
        {
            std::string name;
            std::vector<unsigned char> image;
            unsigned int width;
            unsigned int height;
            unsigned int handle;
            unsigned int uniform;
            int wrapS;
            int wrapT;
        };

        Textures();
        ~Textures();
        std::string add(std::string file, std::string name);
        void setTextures(std::list<Configuration::Texture> textures);
        std::list<Textures::Texture> textures;
    };

} // namespace DemoSystem

#endif