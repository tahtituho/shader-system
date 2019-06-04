#ifndef DYNAMICTEXTURE_H
#define DYNAMICTEXTURE_H
#include <vector>
#include "Texture.h"

namespace DemoSystem {
    class DynamicTexture : public Texture {
        public:
            DynamicTexture();    
            ~DynamicTexture();

            std::string source;
            bool once;
    };
}
#endif