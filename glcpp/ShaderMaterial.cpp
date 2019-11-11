#include <iostream>
#include <algorithm>

#include "glcpp.hpp"
using namespace glcpp;

GLuint ShaderMaterial::create_program(const char *vertexfile, const char *fragmentfile) {
    program = glCreateProgram();
    GLuint shader;

    if(vertexfile) {
        shader = create_shader(vertexfile, GL_VERTEX_SHADER);
        if(!shader)
            return 0;
        glAttachShader(program, shader);
    }

    if(fragmentfile) {
        shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
        if(!shader)
            return 0;
        glAttachShader(program, shader);
    }

    link_program();
    return program;
}

ShaderMaterial::ShaderMaterial(const char* vert, const char* frag){
    program = create_program(vert, frag);
    init_uniforms();
}

void ShaderMaterial::render(){
    glUseProgram(program);

    for( auto set: uniformSet){
        if(!set.second) std::cerr<<"Uniform "<<set.first<<" not initialized"<<std::endl;
    }

    if(blending){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else glDisable(GL_BLEND);

    if(depthTest)glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    set_textures();
};

GLuint ShaderMaterial::getProgramId(){
    return program;
};



