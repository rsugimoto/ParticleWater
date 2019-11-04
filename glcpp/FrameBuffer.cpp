#include "glcpp.hpp"
#include <cstdio>

using namespace glcpp;

 FrameBuffer::FrameBuffer(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format, bool enableDepth){
    glGenFramebuffers(1, &frameBufferId);
    attachAdditionalBuffer(width, height, minFilter, magFilter, wrapS, wrapT, internalFormat, format);
    if(enableDepth)attachDepthBuffer();
}

FrameBuffer::~FrameBuffer(){
    glDeleteFramebuffers(1, &frameBufferId);
    for (auto* texture: textures)
        delete texture;
    if (depthrenderbuffer != UINT_MAX)
        glDeleteRenderbuffers(1, &depthrenderbuffer);
}

void FrameBuffer::attachDepthBuffer(){
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, textures[0]->width, textures[0]->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
}

void FrameBuffer::attachAdditionalBuffer(GLint internalFormat, GLenum format){
    glBindTexture(GL_TEXTURE_2D, textures[0]->getTextureId());
    auto width = textures[0]->width, height = textures[0]->height;
    GLint minFilter, magFilter, wrapS, wrapT;
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &magFilter);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &minFilter);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrapS);
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrapT);
    
    attachAdditionalBuffer(width, height, minFilter, magFilter, wrapS, wrapT, internalFormat, format);
}

void FrameBuffer::attachAdditionalBuffer(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format){
    Texture* texture = new Texture(width, height, minFilter, magFilter, wrapS, wrapT, internalFormat, format);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glFramebufferTexture(GL_FRAMEBUFFER, (GLenum)((uint32_t)GL_COLOR_ATTACHMENT0+textures.size()), texture->getTextureId(),0);
    textures.push_back(texture);
    GLenum* drawBuffers = new GLenum[textures.size()];
    for (int i=0; i<textures.size(); i++) drawBuffers[i] = (GLenum)(GL_COLOR_ATTACHMENT0+i);
    glDrawBuffers(textures.size(), drawBuffers);
    delete[] drawBuffers;
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)fprintf(stderr,"FrameBuffer Error\n");
}

void FrameBuffer::setRenderTarget(){
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
    glViewport(0,0,textures[0]->width,textures[0]->height);
}