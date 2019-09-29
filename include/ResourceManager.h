#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "lodepng.h"
#include <string>
#include <list>
#include <vector>
#include "Configuration.h"
#include "StaticTexture.h"
#include "DynamicTexture.h"
#include "Shader.h"

namespace DemoSystem  {
    class ResourceManager {
        public:
            ResourceManager();
            ~ResourceManager();

            void setResources(std::list<DemoSystem::Asset> assets);
            std::vector<DemoSystem::StaticTexture> staticTextures;
            std::vector<DemoSystem::DynamicTexture> dynamicTextures;

            DemoSystem::Shader primaryShader;
    };

}

#endif