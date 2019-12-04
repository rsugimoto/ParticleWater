#version 430

layout(location=0) out vec4 fragColor;
in vec3 vColor;
vec3 light = normalize(vec3(0.5, -0.5, 1.0));

void main(void){
    vec3 normal;
    normal.xy = gl_PointCoord*2.0 - 1.0;
    float radius = length(normal.xy);
    if (radius>1.0) discard;
    normal.z = sqrt(1.0 - radius*radius);
    float diffuse = max(0.0, dot(normal, light));

    fragColor = vec4(vColor, 1.0) * diffuse;
}