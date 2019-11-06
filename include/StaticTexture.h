#ifndef STATICTEXTURE_H
#define STATICTEXTURE_H
#include <vector>
#include "Texture.h"

namespace DemoSystem {
    class StaticTexture : public Texture {
        public:
            StaticTexture();    
            ~StaticTexture();
            void initialize();
            std::vector<unsigned char> image;
    };
}
#endif