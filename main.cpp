/*
 * Author: Ryusuke Sugimoto
 */

#include <iostream>

//Wrapper of OpenGL written by Ryusuke
#include "glcpp/glcpp.hpp"
using namespace glcpp;

#include "particle_simulator.hpp"
#include "particle_renderer.hpp"

const int width = 640;
const int height = 480;

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

// void idle(){glutPostRedisplay();}
// void keyboardHandler(unsigned char key, int x, int y){exit(0);}

// void render(){
//     glm::mat4 mvpMatrix; //Identity matrix;

//     std::vector<glm::vec3> positions(3);
//     positions[0] = glm::vec3(-0.5,-0.5,0.0);
//     positions[1] = glm::vec3(0.5,-0.5,0.0);
//     positions[2] = glm::vec3(0.0,0.73,0.0);

//     ShaderStorageBuffer* ssbo = new ShaderStorageBuffer(positions);

//     GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
//     // memcpy(buffer.data(), p ,buffer.size()*sizeof(glm::vec4));
//     // for (auto& value: buffer) std::cout<<glm::to_string(value)<<" ";

//     ParticleRenderer* renderer = new ParticleRenderer(width, height);
//     renderer->material->setUniform("mvpMatrix", mvpMatrix);
//     renderer->render(ssbo->buffer_idx);

//     delete ssbo;
//     delete renderer;
// }

int main(int argc, char* argv[]) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("ParticleWater");
    print_compute_shader_limits();

    glm::mat4 mvpMatrix = glm::mat4(1.0); //Identity matrix;

    std::vector<glm::vec4> positions(3);
    positions[0] = glm::vec4(0);
    positions[1] = glm::vec4(0);
    positions[2] = glm::vec4(0);

    ShaderStorageBuffer* ssbo = new ShaderStorageBuffer(positions);
    ComputeShaderProgram* cs = new ComputeShaderProgram("../shaders/test.comp");
    cs->setShaderStorageBuffer("shader_data", ssbo);
    cs->dispatchCompute(1,1,1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    float buffer[16];
    memcpy(buffer, p ,sizeof(buffer));
    for (auto& value: buffer) std::cout<<value<<" ";
    std::cout<<std::endl;
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    ParticleRenderer* renderer = new ParticleRenderer(width, height);
    renderer->material->setUniform("mvpMatrix", mvpMatrix);
    renderer->render(ssbo->buffer_idx); 

    delete cs;
    delete ssbo;
    delete renderer;

    while(1);


    // glutKeyboardFunc(keyboardHandler);
    // glutDisplayFunc(render);
    // glutIdleFunc(idle);
    // glutMainLoop();

    // ShaderStorageBuffer* ssbo = new ShaderStorageBuffer(buffer);
    // ComputeShaderProgram* cs = new ComputeShaderProgram("../shaders/test.comp");
    // cs->setShaderStorageBuffer("shader_data", ssbo);
    // cs->dispatchCompute(1,1,1);
    
    // GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
    // memcpy(buffer.data(), p ,buffer.size()*sizeof(glm::vec4));
    // for (auto& value: buffer) std::cout<<glm::to_string(value)<<" ";

    // delete cs;
    // delete ssbo;



    return 0;
}
