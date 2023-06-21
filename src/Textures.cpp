#include "Textures.h"
#include "Helpers.h"

DemoSystem::Textures::Textures()
{
}

void DemoSystem::Textures::setTextures(std::list<Configuration::Texture> assets)
{
    for (Configuration::Texture asset : assets)
    {
        DemoSystem::Textures::Texture t;
        t.name = asset.name;
        std::vector<unsigned char> buffer;
        lodepng::load_file(buffer, asset.file);
        unsigned error = lodepng::decode(t.image, t.width, t.height, buffer);
        if (error != 0)
        {
            continue;
        }
        t.wrapS = Helpers::mapWrapToOpenGL(asset.wrapS);
        t.wrapT = Helpers::mapWrapToOpenGL(asset.wrapT);
        this->textures.push_back(t);
    }
}

DemoSystem::Textures::~Textures()
{
}