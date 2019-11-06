#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H
#include <GL/glew.h>
#include "lodepng.h"
#include <string>
#include <map>
#include "Configuration.h"
#include "StaticTexture.h"
#include "DynamicTexture.h"
#include "Common.h"

namespace DemoSystem  {
    class TextureManager {
        public:
            TextureManager();
            ~TextureManager();

            void setTextures(std::list<DemoSystem::Common::Asset> assets);
            void initializeTexture(std::string texture);
            void initializeTextures();
            DemoSystem::Texture* getTexture(std::string);
            
        private:
            std::map<std::string, DemoSystem::Texture*> textures;
    };

}

#endif