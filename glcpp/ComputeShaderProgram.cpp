#include <iostream>

#include "glcpp.hpp"
using namespace glcpp;

ComputeShaderProgram::ComputeShaderProgram(const char* comp){
    program = create_program(comp);
    init_uniforms();
}

ComputeShaderProgram::~ComputeShaderProgram(){

}

GLuint ComputeShaderProgram::create_program(const char *comp) {
    program = glCreateProgram();
    GLuint shader;

    if(comp){
        shader = create_shader(comp, GL_COMPUTE_SHADER);
        if(!shader)
            return 0;
        glAttachShader(program, shader);
    }

    link_program();
    return program;
}

void ComputeShaderProgram::dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z){
    glUseProgram(program);

    for( auto set: uniformSet){
        if(!set.second) std::cerr<<"Uniform "<<set.first<<" not initialized"<<std::endl;
    }

    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}