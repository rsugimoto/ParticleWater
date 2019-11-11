#include "glcpp/glcpp.hpp"

class ParticleRenderer{
    public:
        glcpp::ShaderMaterial* material;

        ParticleRenderer(int width, int height);
        ~ParticleRenderer();
        void render(GLuint ibo);
};