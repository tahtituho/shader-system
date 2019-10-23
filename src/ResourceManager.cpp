#include "ResourceManager.h"


DemoSystem::ResourceManager::ResourceManager() {

}

void DemoSystem::ResourceManager::setResources(std::list<DemoSystem::Common::Asset> assets) {
    for(DemoSystem::Common::Asset asset : assets) {
        if(asset.type == DemoSystem::Common::Asset::AssetType::STATIC_TEXTURE) {
            DemoSystem::StaticTexture t;
            t.name = asset.name;
            std::vector<unsigned char> buffer;
            lodepng::load_file(buffer, asset.file);
            unsigned error = lodepng::decode(t.image, t.width, t.height, buffer);
            if(error == 0) {
                this->staticTextures.push_back(t);
            }
        }
        else if(asset.type == DemoSystem::Common::Asset::AssetType::DYNAMIC_TEXTURE) {
            DemoSystem::DynamicTexture t;
            t.name = asset.name;
            t.once = asset.once;
            t.source = "";
            t.width = asset.width;
            t.height = asset.height;
            this->dynamicTextures.push_back(t);
        }

    }
}

DemoSystem::ResourceManager::~ResourceManager() {

}