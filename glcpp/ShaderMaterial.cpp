#include <iostream>
#include <algorithm>

#include "glcpp.hpp"
using namespace glcpp;

GLuint ShaderMaterial::create_program(const char *vertexfile, const char *fragmentfile) {
    program = glCreateProgram();
    GLuint shader;

    if(vertexfile) {
        shader = create_shader(vertexfile, GL_VERTEX_SHADER);
        if(!shader)
            return 0;
        glAttachShader(program, shader);
    }

    if(fragmentfile) {
        shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
        if(!shader)
            return 0;
        glAttachShader(program, shader);
    }

    link_program();
    return program;
}

ShaderMaterial::ShaderMaterial(const char* vert, const char* frag){
    program = create_program(vert, frag);
    init_uniforms();
}

void ShaderMaterial::setUniform(const char* uni, Texture* val){
    glUseProgram(program);
    if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);
    else {
        uniformSet[uni]=true;
        textureIds[uni] = val->getTextureId();
        generateMipmap[val->getTextureId()]=val->generateMipmap;
        auto itr = textureIds.find(uni);
        auto index = std::distance(textureIds.begin(), itr);
        glUniform1i(uniforms[uni], (GLint)index);
    }
};
void ShaderMaterial::setUniform(const char* uni, FrameBuffer* val){this->setUniform(uni, val->textures[0]);};
void ShaderMaterial::render(){
    glUseProgram(program);

    for( auto set: uniformSet){
        if(!set.second) std::cerr<<"Uniform "<<set.first<<" not initialized"<<std::endl;
    }

    if(blending){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else glDisable(GL_BLEND);

    if(depthTest)glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);

    for(auto itr = textureIds.begin(); itr!=textureIds.end(); ++itr){
        auto i = (int)std::distance(textureIds.begin(), itr);
        glActiveTexture((GLenum)(GL_TEXTURE0+i));
        glBindTexture(GL_TEXTURE_2D, itr->second);
        if(generateMipmap[itr->second])glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout<<"Bind texture "<<itr->second<<" to GL_TEXTURE"<<i<<std::endl;
    }
};

GLuint ShaderMaterial::getProgramId(){
    return program;
};



