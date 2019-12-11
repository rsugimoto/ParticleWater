#version 430

in vec4 position;
in float density;
uniform mat4 mvpMatrix;
out vec3 vColor;

void main(void){
    float weight = density/3.0;
    vColor = (1-weight)* vec3(1.0,1.0,1.0) + weight*vec3(0.0, 0.0, 1.0);

    gl_Position = mvpMatrix* vec4(position);
    gl_PointSize = 15.0/gl_Position.w;
}