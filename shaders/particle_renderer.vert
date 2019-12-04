#version 430

in vec4 position;
uniform mat4 mvpMatrix;
out vec3 vColor;

void main(void){
    vColor = vec3(0.5, 0.5, 1.0);

    gl_Position = mvpMatrix* vec4(position);
    gl_PointSize = 20.0/gl_Position.w;
}