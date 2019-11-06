#include "TextureManager.h"


DemoSystem::TextureManager::TextureManager() {

}

void DemoSystem::TextureManager::setTextures(std::list<DemoSystem::Common::Asset> assets) {
    for(DemoSystem::Common::Asset asset : assets) {
        if(asset.type == DemoSystem::Common::Asset::AssetType::STATIC_TEXTURE) {
            DemoSystem::StaticTexture* t = new DemoSystem::StaticTexture();
            std::vector<unsigned char> buffer;
            lodepng::load_file(buffer, asset.file);
            unsigned error = lodepng::decode(t->image, t->width, t->height, buffer);
            if(error == 0) {
                this->textures[asset.name] = t;
            }
        }
        else if(asset.type == DemoSystem::Common::Asset::AssetType::DYNAMIC_TEXTURE) {
            DemoSystem::DynamicTexture* t = new DemoSystem::DynamicTexture();
            t->once = asset.once;
            t->source = "";
            t->width = asset.width;
            t->height = asset.height;
            this->textures[asset.name] = t;
        }

    }
}

void DemoSystem::TextureManager::initializeTexture(std::string name) {
    this->textures[name]->initialize();
}

void DemoSystem::TextureManager::initializeTextures() {
    for(std::pair<std::string, DemoSystem::Texture*> t : this->textures) {
        t.second->initialize();   
    }
}

DemoSystem::Texture* DemoSystem::TextureManager::getTexture(std::string name) {
    return this->textures[name];
}

DemoSystem::TextureManager::~TextureManager() {

}