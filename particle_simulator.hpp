#include "glcpp/glcpp.hpp"
using namespace glcpp;

class ParticleSimulator {
    private:
        const int num_particles = 65535;
        const int bucket_dim = 1024;
        const float cutoff_dist = 1.0;
        bool odd_iteration = true;

        ShaderStorageBuffer *positionBuffer1, *velocityBuffer1, *densityBuffer1,
                            *positionBuffer2, *velocityBuffer2, *densityBuffer2,
                            *bucketBuffer, *distanceFuncBuffer, *wallWeightBuffer;
        ComputeShaderProgram *bucketGenerationShader, *densityComputaionShader, *velocityUpdateShader, *positionUpdateShader;
    public:
        ParticleSimulator();
        ~ParticleSimulator();
        void update(float timestep);
        GLuint getPositionBufferObject();
};