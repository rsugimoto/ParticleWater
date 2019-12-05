#include "particle_simulator.hpp"
#include <iostream>

ParticleSimulator::ParticleSimulator(unsigned int num_particles, unsigned int bucket_res){
    this->num_particles = num_particles;
    this->bucket_res = bucket_res;
    assert(bucket_res<=256);
    effective_radius = 1.0/bucket_res;
    dispatch_x = (int)std::ceil(num_particles/32.0)%65536;
    dispatch_y = (int)std::ceil(num_particles/(32.0*65536))%65536;
    odd_iteration = true;

    positionBuffer1 = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat));
    velocityBuffer1 = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat));
    positionBuffer2 = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat));
    velocityBuffer2 = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat));
    densityBuffer = new ShaderStorageBuffer(num_particles*1*sizeof(GLfloat));
    bucketBuffer = new ShaderStorageBuffer((unsigned long)bucket_res*(unsigned long)bucket_res*(unsigned long)bucket_res*16*sizeof(GLuint), GL_DYNAMIC_COPY, GL_R32UI);
    //distanceFuncBuffer = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat), GL_STATIC_READ); //TO BE CHANGED
    //wallWeightBuffer = new ShaderStorageBuffer(num_particles*4*sizeof(GLfloat), GL_STATIC_READ); //TO BE CHANGED

    bucketGenerationShader = new ComputeShaderProgram("../shaders/bucket.comp");
    densityComputaionShader = new ComputeShaderProgram("../shaders/density.comp");
    velPosUpdateShader = new ComputeShaderProgram("../shaders/velpos.comp");

    bucketGenerationShader->setShaderStorageBuffer("bucket", bucketBuffer);
    densityComputaionShader->setShaderStorageBuffer("bucket", bucketBuffer);
    velPosUpdateShader->setShaderStorageBuffer("bucket", bucketBuffer);
    densityComputaionShader->setShaderStorageBuffer("densities", densityBuffer);
    velPosUpdateShader->setShaderStorageBuffer("densities", densityBuffer);
    //densityComputaionShader->setShaderStorageBuffer("distance_function", distanceFuncBuffer);
    //densityComputaionShader->setShaderStorageBuffer("wall_weight_function", wallWeightBuffer); //Needs Precomputation
    //velPosUpdateShader->setShaderStorageBuffer("distance_function", distanceFuncBuffer);
    bucketGenerationShader->setUniform("bucket_res", (GLuint)bucket_res);
    densityComputaionShader->setUniform("bucket_res", (GLuint)bucket_res);
    velPosUpdateShader->setUniform("bucket_res", (GLuint)bucket_res);
    bucketGenerationShader->setUniform("num_particles", (GLuint)num_particles);
    densityComputaionShader->setUniform("num_particles", (GLuint)num_particles);
    velPosUpdateShader->setUniform("num_particles", (GLuint)num_particles);
    densityComputaionShader->setUniform("re", (GLdouble)effective_radius);
    densityComputaionShader->setUniform("re2", (GLdouble)pow(effective_radius,2));
    densityComputaionShader->setUniform("re9", (GLdouble)pow(effective_radius,9));
    velPosUpdateShader->setUniform("re", (GLdouble)effective_radius);
    velPosUpdateShader->setUniform("re6", (GLdouble)pow(effective_radius,6));
   }

ParticleSimulator::~ParticleSimulator(){
    delete positionBuffer1;
    delete velocityBuffer1;
    delete positionBuffer2;
    delete velocityBuffer2;
    delete densityBuffer;
    delete bucketBuffer;
    // delete distanceFuncBuffer;
    // delete wallWeightBuffer;

    delete bucketGenerationShader;
    delete densityComputaionShader;
    delete velPosUpdateShader;
}

void ParticleSimulator::setInitParticlePositions(){
    double dam_fill_rate = 0.2;
    auto initPosShader = new ComputeShaderProgram("../shaders/init_pos.comp");
    initPosShader->setShaderStorageBuffer("positions_1", positionBuffer1);
    initPosShader->setShaderStorageBuffer("positions_2", positionBuffer2);
    initPosShader->setUniform("num_particles", (GLuint)num_particles);
    initPosShader->setUniform("dam_fill_rate", (GLfloat)dam_fill_rate);
    initPosShader->dispatchCompute(dispatch_x, dispatch_y, 1);
    
    densityComputaionShader->setUniform("mass", (GLfloat)(1000*dam_fill_rate/num_particles));
    velPosUpdateShader->setUniform("mass", (GLfloat)(1000*dam_fill_rate/num_particles));
    delete initPosShader;
}

void ParticleSimulator::update(float timestep){
    ShaderStorageBuffer *pos_in, *vel_in, *pos_out, *vel_out; 
    if(odd_iteration){
        pos_in = positionBuffer1;
        pos_out = positionBuffer2;
        vel_in = velocityBuffer1;
        vel_out = velocityBuffer2;
    }else{
        pos_in = positionBuffer2;
        pos_out = positionBuffer1;
        vel_in = velocityBuffer2;
        vel_out = velocityBuffer1;
    }
    odd_iteration = !odd_iteration;

    bucketBuffer->setValue(num_particles);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    bucketGenerationShader->setShaderStorageBuffer("positions_in", pos_in);
    bucketGenerationShader->dispatchCompute(dispatch_x, dispatch_y,1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    densityComputaionShader->setShaderStorageBuffer("positions_in", pos_in);
    densityComputaionShader->dispatchCompute(dispatch_x, dispatch_y,1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    velPosUpdateShader->setShaderStorageBuffer("positions_in", pos_in);
    velPosUpdateShader->setShaderStorageBuffer("positions_out", pos_out);
    velPosUpdateShader->setShaderStorageBuffer("velocities_in", vel_in);
    velPosUpdateShader->setShaderStorageBuffer("velocities_out", vel_out);
    velPosUpdateShader->setUniform("timestep", (GLfloat)timestep);
    velPosUpdateShader->dispatchCompute(dispatch_x, dispatch_y,1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

}

GLuint ParticleSimulator::getPositionBufferObject(){
    if(odd_iteration) return positionBuffer2->buffer_idx;
    else return positionBuffer1->buffer_idx;
}