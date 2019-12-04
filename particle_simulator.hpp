#include "glcpp/glcpp.hpp"
#include <cmath>
#include <cassert>
using namespace glcpp;

class ParticleSimulator {
    private:
        unsigned int num_particles, bucket_res, dispatch_x, dispatch_y;
        double effective_radius;
        bool odd_iteration;

        ShaderStorageBuffer *positionBuffer1, *velocityBuffer1, *positionBuffer2, *velocityBuffer2,
                            *densityBuffer, *bucketBuffer;//, *distanceFuncBuffer, *wallWeightBuffer;
        ComputeShaderProgram *bucketGenerationShader, *densityComputaionShader, *velPosUpdateShader;
    public:
        ParticleSimulator(unsigned int num_particles=32, unsigned int bucket_res=1024);
        ~ParticleSimulator();
        void update(float timestep);
        GLuint getPositionBufferObject();
        void setInitParticlePositions();
};