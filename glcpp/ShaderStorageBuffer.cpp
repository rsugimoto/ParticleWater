#include "glcpp.hpp"
using namespace glcpp;

unsigned int ShaderStorageBuffer::buffer_count = 0;

ShaderStorageBuffer::ShaderStorageBuffer(GLsizeiptr size, GLenum usage){
    glGenBuffers(1, &buffer_idx);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_idx);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, usage);
    storage_block_binding = buffer_count;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_idx);
    buffer_count++;
}

ShaderStorageBuffer::ShaderStorageBuffer(std::vector<float> data, GLenum usage){
    glGenBuffers(1, &buffer_idx);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_idx);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size()*sizeof(float), data.data(), usage);
    storage_block_binding = buffer_count;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_idx);
    buffer_count++;
}

ShaderStorageBuffer::ShaderStorageBuffer(std::vector<glm::vec3> data, GLenum usage){
    glGenBuffers(1, &buffer_idx);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_idx);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size()*sizeof(glm::vec3), glm::value_ptr(data.front()), usage);
    storage_block_binding = buffer_count;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_idx);
    buffer_count++;
}

ShaderStorageBuffer::ShaderStorageBuffer(std::vector<glm::vec4> data, GLenum usage){
    glGenBuffers(1, &buffer_idx);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer_idx);
    glBufferData(GL_SHADER_STORAGE_BUFFER, data.size()*sizeof(glm::vec4), glm::value_ptr(data.front()), usage);
    storage_block_binding = buffer_count;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, storage_block_binding, buffer_idx);
    buffer_count++;
}

ShaderStorageBuffer::~ShaderStorageBuffer(){
    glDeleteBuffers(1, &buffer_idx);
}

GLuint ShaderStorageBuffer::getStorageBlockBinding(){
    return storage_block_binding;
}