#include "Textures.h"


DemoSystem::Textures::Textures() {

}

void DemoSystem::Textures::setTextures(std::list<DemoSystem::Asset> textures) {
    for(DemoSystem::Asset asset : textures) {
        DemoSystem::Texture t;
        t.name = asset.name;
        std::vector<unsigned char> buffer;
        lodepng::load_file(buffer, asset.file);
        unsigned error = lodepng::decode(t.image, t.width, t.height, buffer);
        if(error == 0) {
            this->textures.push_back(t);
        }
        //return std::string(lodepng_error_text(error));
    }
}

DemoSystem::Textures::~Textures() {

}