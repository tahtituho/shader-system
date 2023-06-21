#include "Framebuffer.h"

DemoSystem::Framebuffer::Framebuffer() {
}

DemoSystem::Framebuffer::~Framebuffer() {
    this->cleanFramebuffer();
}

GLuint DemoSystem::Framebuffer::getFBO() {
    return this->fbo;
}

void DemoSystem::Framebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
}

void DemoSystem::Framebuffer::unBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DemoSystem::Framebuffer::drawFBO() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // Have no idea why this bind is done here, but it works for some reason...
    glBindVertexArray(this->vto);
    glUseProgram(this->program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->fbo_texture);
    glUniform1i(this->uniform_mainImage, 0);
    glEnableVertexAttribArray(this->attribute_position_postproc);

    // Tracked uniforms
    for (auto tv = this->synchronizer->trackVariables.begin(); tv != this->synchronizer->trackVariables.end(); tv++)
    {
        if (tv->shaderType == TrackVariable::ShaderType::POST)
        {
            if (tv->type == Variable::DataType::FLOAT1)
            {
                glUniform1f(tv->uniform, (GLfloat)tv->value.x);
            }
            else if (tv->type == Variable::DataType::FLOAT2)
            {
                glUniform2f(tv->uniform, (GLfloat)tv->value.x, (GLfloat)tv->value.y);
            }
            else if (tv->type == Variable::DataType::FLOAT3)
            {
                glUniform3f(tv->uniform, (GLfloat)tv->value.x, (GLfloat)tv->value.y, (GLfloat)tv->value.z);
            }
        }
    }

    unsigned int textureIndex = 1;

    for (auto it = this->textures->begin(); it != this->textures->end(); it++)
    {
        glActiveTexture(GL_TEXTURE0 + textureIndex);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glUniform1i(it->uniform, textureIndex);
        textureIndex++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glVertexAttribPointer(
        attribute_position_postproc,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(attribute_position_postproc);

    // glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    // glBindVertexArray(this->vto);
    // This crashes the program. Maybe the drawing is incorrect? Is it trying to draw something wrong?
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
}

void DemoSystem::Framebuffer::generateFBO(unsigned int width, unsigned int height) {
    /* Texture */
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &this->fbo_texture);
    glBindTexture(GL_TEXTURE_2D, this->fbo_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Depth buffer */
    glGenRenderbuffers(1, &this->rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    /* Framebuffer to link everything together */
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbo_depth);
    GLenum status;
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        this->logger->write(DemoSystem::Logger::ERR, "FBO FAILURE");
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Init vertices
    glGenBuffers(1, &this->vbo_fbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_fbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->fbo_vertices), this->fbo_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &this->vto);
    glBindVertexArray(this->vto);
    this->logger->write(DemoSystem::Logger::INFO, "FBO SUCCESS");
}

void DemoSystem::Framebuffer::addUniformsPost() 
{
    // PostProcessing specific uniforms
    const char* uniform_name;
    uniform_name = "position";
    this->attribute_position_postproc = glGetAttribLocation(this->program, uniform_name);
    if (this->attribute_position_postproc == -1) {
        this->logger->write(DemoSystem::Logger::ERR, "Could not bind position uniform");
    }
    uniform_name = "mainImage";
    this->uniform_mainImage = glGetUniformLocation(this->program, uniform_name);
    if (this->uniform_mainImage == -1) {
        this->logger->write(DemoSystem::Logger::ERR, "Could not bind main image uniform");
    }
    // Tracked uniforms
    for (auto tv = this->synchronizer->trackVariables.begin(); tv != this->synchronizer->trackVariables.end(); tv++)
    {
        if (tv->shaderType == TrackVariable::ShaderType::POST)
        {
            tv->uniform = glGetUniformLocation(this->program, tv->name.c_str());
        }
    }

    for (auto it = this->textures->begin(); it != this->textures->end(); it++)
    {
        glGenTextures(1, &it->handle);
        glBindTexture(GL_TEXTURE_2D, it->handle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, it->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, it->wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, it->width, it->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &it->image[0]);
        it->uniform = glGetUniformLocation(this->program, it->name.c_str());
    }
}

void DemoSystem::Framebuffer::resizeFBO(unsigned int width, unsigned int height) {
    glBindTexture(GL_TEXTURE_2D, this->fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, this->rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void DemoSystem::Framebuffer::cleanFramebuffer() {
    glDeleteFramebuffers(1, &this->fbo);
    glDeleteTextures(1, &this->fbo_texture);
    glDeleteTextures(1, &this->rbo_depth);
    glDeleteBuffers(1, &this->vbo_fbo_vertices);
}