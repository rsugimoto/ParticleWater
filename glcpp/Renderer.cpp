#include "glcpp.hpp"
using namespace glcpp;

Renderer::Renderer(int width, int height){
    this->width = width;
    this->height = height;
}
void Renderer::render(BufferGeometry* geometry, ShaderMaterial* material, FrameBuffer* renderTarget){
    if(renderTarget==nullptr){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,width,height);
    }
    else renderTarget->setRenderTarget();
    glCullFace(GL_FRONT);
    if(material->wireFrame)glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    material->render();
    geometry->render(material->getProgramId());
    glFinish();
    if(material->wireFrame)glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
};