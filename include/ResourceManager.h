#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "lodepng.h"
#include <string>
#include <list>
#include <vector>
#include "Configuration.h"
#include "Texture.h"

namespace DemoSystem  {
    class ResourceManager {
        public:
            ResourceManager();
            ~ResourceManager();

            void setResources(std::list<DemoSystem::Asset> assets);
            std::vector<DemoSystem::Texture> textures;
    };

}

#endif