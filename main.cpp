/*
 * Author: Ryusuke Sugimoto
 */

#include <iostream>
#include <chrono>

//Wrapper of OpenGL written by Ryusuke
#include "glcpp/glcpp.hpp"
using namespace glcpp;

#include "particle_simulator.hpp"
#include "particle_renderer.hpp"

const int width = 640;
const int height = 480;

int main(int argc, char* argv[]) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("ParticleWater");
    std::cout<<glGetString(GL_VERSION)<<std::endl;

    uint particle_num = 8192;
    uint bucket_res = 256;
    if(argc>1) particle_num = atoi(argv[1]);
    if(argc>2) bucket_res = atoi(argv[2]);
    ParticleSimulator* simulator = new ParticleSimulator(particle_num, bucket_res);
    ParticleRenderer* renderer = new ParticleRenderer(width, height);
    simulator->setInitParticlePositions();
    renderer->render(simulator->getPositionBufferObject());

    auto prev_time = std::chrono::high_resolution_clock::now();
    float frame_rate = 0;
    while(1){
        getchar();

        auto curr_time = std::chrono::high_resolution_clock::now();
        //auto time_passed = std::chrono::duration_cast<std::chrono::duration<float>>(curr_time-prev_time).count();
        float time_passed = 0.01;
        frame_rate = frame_rate*0.05 + (1.0/time_passed)*0.95;
        prev_time = curr_time;
        simulator->update(time_passed);
        renderer->render(simulator->getPositionBufferObject());
        std::cout<<frame_rate<<std::endl;
        //break;
    }
    delete renderer;
    delete simulator;

    while(1);

    return 0;
}
