#include "glcpp/glcpp.hpp"

class ParticleRenderer{
    private:
        glm::vec3 camera_pos = glm::vec3(0.5,1.2,2);
        glm::vec3 camera_lookAt = glm::vec3(0.5,0.4,0.5);
        float fov= 45.0;
        float near = 0.1;
        float far = 10;
    public:
        glcpp::ShaderMaterial* material;

        ParticleRenderer(int width, int height);
        ~ParticleRenderer();
        void render(GLuint pos, GLuint dns);
};