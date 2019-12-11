/*
 * Author: Ryusuke Sugimoto
 */

#include <iostream>
#include <chrono>
#include <signal.h>

//Wrapper of OpenGL written by Ryusuke
#include "glcpp/glcpp.hpp"
using namespace glcpp;

#include "particle_simulator.hpp"
#include "particle_renderer.hpp"

const int width = 960; //1920;
const int height = 540; //1080;

bool end_loop = false;

void signal_handler(int sig){
    end_loop = true;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("ParticleWater");
    //glutFullScreen();
    std::cout<<glGetString(GL_VERSION)<<std::endl;

    uint preset = 1;
    if(argc>1) preset = atoi(argv[1]);
    std::cout<<"Initializing..."<<std::endl;

    ParticleSimulator* simulator = new ParticleSimulator(65536, 32);
    ParticleRenderer* renderer = new ParticleRenderer(width, height);
    simulator->setInitParticlePositions(preset);
    renderer->render(simulator->getPositionBufferObject(), simulator->getDensityBufferObject());
    std::cout<<"Initialization finished."<<std::endl;
    std::cout<<"Press enter to start the simulation. Ctrl+C to exit the program."<<std::endl;
    getchar();
    signal(SIGINT, signal_handler);
    auto prev_time = std::chrono::high_resolution_clock::now();
    float frame_rate = 0;
    while(not end_loop){
        auto curr_time = std::chrono::high_resolution_clock::now();
        auto time_passed = std::chrono::duration_cast<std::chrono::duration<float>>(curr_time-prev_time).count();
        frame_rate = frame_rate*0.05 + (1.0/time_passed)*0.95;
        prev_time = curr_time;
        for (int i=0; i<5; i++) simulator->update(time_passed/25); //adjust the timestep for simulation
        renderer->render(simulator->getPositionBufferObject(), simulator->getDensityBufferObject());
        std::cout<<frame_rate<<std::endl;
        glutSwapBuffers();
    }
    delete renderer;
    delete simulator;
    return 0;
}
