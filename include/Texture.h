#ifndef TEXTURE_H
#define TEXTURE_H
#include <vector>
#include "Resource.h"

namespace DemoSystem {
    class Texture : public Resource {
        public:
            Texture();    
            ~Texture();

            std::vector<unsigned char> image;
            unsigned int width;
            unsigned int height;
            unsigned int handle;
            unsigned int uniform;
    };
}
#endif