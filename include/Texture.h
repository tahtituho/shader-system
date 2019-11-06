#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <string>

namespace DemoSystem {
    class Texture {
        public:
            Texture();    
            virtual ~Texture() = default;
            virtual void initialize() {};
            std::string name;
            GLuint handle;
            unsigned int width;
            unsigned int height;
    };
}
#endif