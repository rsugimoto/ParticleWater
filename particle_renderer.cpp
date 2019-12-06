#include "particle_renderer.hpp"
using namespace glcpp;

ParticleRenderer::ParticleRenderer(int width, int height){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,width,height);
    //glCullFace(GL_FRONT);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    material = new ShaderMaterial("../shaders/particle_renderer.vert","../shaders/particle_renderer.frag");

    glm::mat4 view = glm::lookAt(camera_pos, camera_lookAt, glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(fov), ((float)width)/((float)height), near, far); 
    glm::mat4 mvpMatrix = projection*view;
    material->setUniform("mvpMatrix", mvpMatrix);

}

ParticleRenderer::~ParticleRenderer(){
    delete material;
}

void ParticleRenderer::render(GLuint vbo){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    material->render();
    const char* attribute_name = "position";
    glUseProgram(material->getProgramId());
    GLint attribute_location = glGetAttribLocation(material->getProgramId(), attribute_name);
    if (attribute_location == -1) fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    else{
        glEnableVertexAttribArray((GLuint)attribute_location);
        // Describe our vertices array to OpenGL (it can't guess its format automatically)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
                (GLuint)attribute_location, // attribute
                4,                 // number of elements per vertex, here (x,y,z,w)
                GL_FLOAT,          // the type of each element
                GL_FALSE,          // take our values as-is
                0,                 // no extra data between each position
                0                  // offset of first element
        );
        GLint size;
        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawArrays(GL_POINTS, 0, size/(sizeof(GL_FLOAT)*4));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray((GLuint)attribute_location);
    }
    glFinish();
}