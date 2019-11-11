#include "particle_simulator.hpp"

ParticleSimulator::ParticleSimulator(){
    positionBuffer1 = new ShaderStorageBuffer(num_particles*4*sizeof(float));
    velocityBuffer1 = new ShaderStorageBuffer(num_particles*4*sizeof(float));
    densityBuffer1 = new ShaderStorageBuffer(num_particles*1*sizeof(float));

    positionBuffer2 = new ShaderStorageBuffer(num_particles*4*sizeof(float));
    velocityBuffer2 = new ShaderStorageBuffer(num_particles*4*sizeof(float));
    densityBuffer2 = new ShaderStorageBuffer(num_particles*1*sizeof(float));

    bucketBuffer = new ShaderStorageBuffer(bucket_dim*bucket_dim*bucket_dim*sizeof(float)); //TO BE CHANGED
    distanceFuncBuffer = new ShaderStorageBuffer(num_particles*4*sizeof(float), GL_STATIC_READ); //TO BE CHANGED
    wallWeightBuffer = new ShaderStorageBuffer(num_particles*4*sizeof(float), GL_STATIC_READ); //TO BE CHANGED

    bucketGenerationShader = new ComputeShaderProgram("../shaders/bucket.comp");
    densityComputaionShader = new ComputeShaderProgram("../shaders/density.comp");
    velocityUpdateShader = new ComputeShaderProgram("../shaders/velocity.comp");
    positionUpdateShader = new ComputeShaderProgram("../shaders/position.comp");

    bucketGenerationShader->setShaderStorageBuffer("bucket", bucketBuffer);
    densityComputaionShader->setShaderStorageBuffer("bucket", bucketBuffer);
    densityComputaionShader->setShaderStorageBuffer("distance_function", distanceFuncBuffer);
    densityComputaionShader->setShaderStorageBuffer("wall_weight_function", wallWeightBuffer); //Needs Precomputation
    velocityUpdateShader->setShaderStorageBuffer("distance_function", distanceFuncBuffer);
}

ParticleSimulator::~ParticleSimulator(){
    delete positionBuffer1;
    delete velocityBuffer1;
    delete densityBuffer1;
    delete positionBuffer2;
    delete velocityBuffer2;
    delete densityBuffer2;
    delete bucketBuffer;
    delete distanceFuncBuffer;
    delete wallWeightBuffer;

    delete bucketGenerationShader;
    delete densityComputaionShader;
    delete velocityUpdateShader;
    delete positionUpdateShader;
}

void ParticleSimulator::update(float timestep){
    ShaderStorageBuffer *pos_in, *vel_in, *dns_in, *pos_out, *vel_out, *dns_out; 
    if(odd_iteration){
        pos_in = positionBuffer1;
        pos_out = positionBuffer2;
        vel_in = velocityBuffer1;
        vel_out = velocityBuffer2;
        dns_in = densityBuffer1;
        dns_out = densityBuffer2;
    }else{
        pos_in = positionBuffer2;
        pos_out = positionBuffer1;
        vel_in = velocityBuffer2;
        vel_out = velocityBuffer1;
        dns_in = densityBuffer2;
        dns_out = densityBuffer1;
    }
    odd_iteration = !odd_iteration;

    bucketGenerationShader->setShaderStorageBuffer("positions_in", pos_in);
    bucketGenerationShader->dispatchCompute(1,1,1);

    densityComputaionShader->setShaderStorageBuffer("densities_in", dns_in);
    densityComputaionShader->setShaderStorageBuffer("densities_out", dns_out);
    densityComputaionShader->dispatchCompute(1,1,1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    velocityUpdateShader->setShaderStorageBuffer("densities_in", dns_out);
    velocityUpdateShader->setShaderStorageBuffer("positions_in", pos_in);
    velocityUpdateShader->setShaderStorageBuffer("velocities_in", vel_in);
    velocityUpdateShader->setShaderStorageBuffer("velocities_out", vel_out);
    velocityUpdateShader->dispatchCompute(1,1,1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


    positionUpdateShader->setShaderStorageBuffer("positions_in", pos_in);
    positionUpdateShader->setShaderStorageBuffer("positions_out", pos_out);
    positionUpdateShader->setShaderStorageBuffer("velocities_in", vel_out);
    positionUpdateShader->setUniform("timestep", (GLfloat)timestep);
    positionUpdateShader->dispatchCompute(1,1,1);

    glMemoryBarrier(GL_ALL_BARRIER_BITS);

}

GLuint ParticleSimulator::getPositionBufferObject(){
    if(odd_iteration) return positionBuffer2->buffer_idx;
    else return positionBuffer1->buffer_idx;
}