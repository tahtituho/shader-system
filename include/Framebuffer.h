#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <GL/glew.h>
#include "Shader.h"

namespace DemoSystem {
    class Framebuffer: public DemoSystem::Shader {
        public:
            Framebuffer();   
            ~Framebuffer();
            GLuint getFBO();

            void cleanFramebuffer();
            void generateFBO(unsigned int width, unsigned int height);
            void resizeFBO(unsigned int width, unsigned int height);
            void addUniformsPost();
            void drawFBO();
            void bind();
            void unBind();
        private:  
            GLuint fbo;                   //framebuffer object
            GLuint vto;
		    GLuint fbo_texture;         
		    GLuint rbo_depth;	
            GLuint vbo_fbo_vertices;
            GLfloat fbo_vertices[8] = {
                -1, -1,
                1, -1,
                -1,  1,
                1,  1,
            };
            // uniforms?
            GLuint attribute_position_postproc;
            GLuint uniform_mainImage;
		    //std::vector<GLenum> drawbuffer;     //add texture attachements
    };
}
#endif