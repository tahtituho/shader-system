#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <GL/glew.h>
#include "Graphics.h"

namespace DemoSystem {
    class Framebuffer: public DemoSystem::Graphics {
        public:
            Framebuffer();   
            ~Framebuffer();
            GLuint getFBO();

            void cleanFramebuffer();
            void generateFBO(unsigned int width, unsigned int height);
            void resizeFBO(unsigned int width, unsigned int height);
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
            GLuint attribute_v_coord_postproc;
            GLuint uniform_fbo_texture;
		    //std::vector<GLenum> drawbuffer;     //add texture attachements
    };
}
#endif