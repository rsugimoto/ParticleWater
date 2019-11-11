#version 430

layout(location=0) out vec4 fragColor;

void main(void){
    fragColor = vec4(gl_PointCoord.x,gl_PointCoord.y,0.0, 1.0);
}