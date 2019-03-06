#ifndef TEXTURES_H
#define TEXTURES_H
#include "lodepng.h"
#include <string>
#include <list>
#include <vector>
#include "Configuration.h"

namespace DemoSystem  {
    struct Texture {
        std::string name;
        std::vector<unsigned char> image;
        unsigned int width;
        unsigned int height;
    };

    class Textures {
        public:
            Textures();
            ~Textures();
            std::string add(std::string file, std::string name);
            void setTextures(std::list<DemoSystem::Asset> textures);
            std::vector<DemoSystem::Texture> textures;
    };

}

#endif