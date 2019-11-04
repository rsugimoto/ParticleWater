/*
 * Author: Ryusuke Sugimoto
 */


//Wrapper of OpenGL written by Ryusuke
#include "glcpp/glcpp.hpp"
using namespace glcpp;

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutCreateWindow("ParticleWater");

    Renderer renderer = Renderer(1024, 1024);

    return 0;
}
