/*
 * Author: Ryusuke Sugimoto
 */

#include <iostream>

//Wrapper of OpenGL written by Ryusuke
#include "glcpp/glcpp.hpp"
using namespace glcpp;

void print_compute_shader_limits(){
    std::cout<<glGetString(GL_VERSION)<<std::endl;

    int data;
    std::cout<<"MAX_WORK_GROUP_COUNT: ";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &data);
    std::cout<<data<<"x";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &data);
    std::cout<<data<<"x";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &data);
    std::cout<<data<<"\n";

    std::cout<<"MAX_WORK_GROUP_SIZE: ";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &data);
    std::cout<<data<<"x";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &data);
    std::cout<<data<<"x";
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &data);
    std::cout<<data<<" (";
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &data);
    std::cout<<data<<")\n";
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("ParticleWater");

    print_compute_shader_limits();

    std::vector<glm::vec4> buffer(16);
    ShaderStorageBuffer* ssbo = new ShaderStorageBuffer(buffer);
    ComputeShaderProgram* cs = new ComputeShaderProgram("../shaders/test.comp");
    cs->setShaderStorageBuffer("shader_data", ssbo);
    cs->dispatchCompute(1,1,1);
    
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    memcpy(buffer.data(), p ,buffer.size()*sizeof(glm::vec4));
    for (auto& value: buffer) std::cout<<glm::to_string(value)<<" ";

    delete cs;
    delete ssbo;
    
    return 0;
}
