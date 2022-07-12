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

    glUseProgram(this->program);
    glBindTexture(GL_TEXTURE_2D, this->fbo_texture);
    glUniform1i(uniform_fbo_texture, /*GL_TEXTURE*/0);
    //glEnableVertexAttribArray(attribute_v_coord_postproc);
    glEnableVertexAttribArray(attribute_v_coord_postproc);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glVertexAttribPointer(
        attribute_v_coord_postproc,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0                   // offset of first element
    );
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(attribute_v_coord_postproc);

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