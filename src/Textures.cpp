#include "Textures.h"

DemoSystem::Textures::Textures()
{
}

void DemoSystem::Textures::setTextures(std::list<Configuration::Asset> assets)
{
    std::list<Configuration::Asset> assetTextures(assets);
    assetTextures.remove_if([](Configuration::Asset a) {
        return a.type != Configuration::Asset::AssetType::TEXTURE;
    });

    for (Configuration::Asset asset : assetTextures)
    {
        DemoSystem::Textures::Texture t;
        t.name = asset.name;
        std::vector<unsigned char> buffer;
        lodepng::load_file(buffer, asset.file);
        unsigned error = lodepng::decode(t.image, t.width, t.height, buffer);
        if (error == 0)
        {
            this->textures.push_back(t);
        }
    }
}

DemoSystem::Textures::~Textures()
{
}