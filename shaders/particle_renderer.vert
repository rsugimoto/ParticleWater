#version 430

in vec4 position;
in float density;
uniform mat4 mvpMatrix;
out vec3 vColor;

void main(void){
    vColor = density/10.0* vec3(1.0,1.0,1.0) + vec3(0, 0, 1);

    gl_Position = mvpMatrix* vec4(position);
    gl_PointSize = 20.0/gl_Position.w;
}