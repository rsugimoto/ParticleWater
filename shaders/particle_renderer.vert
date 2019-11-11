#version 430

in vec4 position;
uniform mat4 mvpMatrix;

void main(void){
    gl_Position = mvpMatrix* vec4(position);
    gl_PointSize = 64.0;
}