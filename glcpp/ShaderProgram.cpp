#include <iostream>

#include "glcpp.hpp"
using namespace glcpp;

ShaderProgram::ShaderProgram(){}
ShaderProgram::~ShaderProgram(){glDeleteProgram(program);}

/**
 * Store all the file's contents in memory, useful to pass shaders
 * source code to OpenGL
 */
char* ShaderProgram::file_read(const char* filename)
{
    FILE* in = fopen(filename, "rb");
    if (in == NULL) return NULL;

    int res_size = BUFSIZ;
    char* res = (char*)malloc(res_size);
    int nb_read_total = 0;

    while (!feof(in) && !ferror(in)) {
        if (nb_read_total + BUFSIZ > res_size) {
            if (res_size > 10*1024*1024) break;
            res_size = res_size * 2;
            res = (char*)realloc(res, res_size);
        }
        char* p_res = res + nb_read_total;
        nb_read_total += fread(p_res, 1, BUFSIZ, in);
    }

    fclose(in);
    res = (char*)realloc(res, nb_read_total + 1);
    res[nb_read_total] = '\0';
    return res;
}

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void ShaderProgram::print_log(GLuint object)
{
    GLint log_length = 0;
    if (glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if (glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        fprintf(stderr, "printlog: Not a shader or a program\n");
        return;
    }

    char* log = (char*)malloc(log_length);

    if (glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
    else if (glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    fprintf(stderr, "%s", log);
    free(log);
}

/**
 * Compile the shader from file 'filename', with error handling
 */
GLuint ShaderProgram::create_shader(const char* filename, GLenum type)
{
    const GLchar* source = file_read(filename);
    if (source == NULL) {
        fprintf(stderr, "Error opening %s: ", filename); perror("");
        return 0;
    }
    GLuint res = glCreateShader(type);
    const GLchar* sources[] = { source };
    glShaderSource(res, 1, sources, NULL);
    free((void*)source);

    glCompileShader(res);
    GLint compile_ok = GL_FALSE;
    glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
    if (compile_ok == GL_FALSE) {
        fprintf(stderr, "%s:", filename);
        print_log(res);
        glDeleteShader(res);
        return 0;
    }

    return res;
}

GLuint ShaderProgram::link_program()
{
    glLinkProgram(program);
    GLint link_ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        fprintf(stderr, "glLinkProgram:");
        print_log(program);
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void ShaderProgram::init_uniforms(){
    GLint active_uniform_count = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &active_uniform_count);
    const std::string ext("[0]");
    for (int i=0; i<active_uniform_count; ++i){
        GLsizei length;
        GLsizei size;
        GLenum type;
        GLchar name[64];
        glGetActiveUniform(program, (GLuint)i, 64, &length, &size, &type, name);
        std::string key = name;
        if (key != ext && key.size()>ext.size()&&key.substr(key.size()-ext.size())==ext) key = key.substr(0, key.size()-ext.size());
        this->uniforms[key] = glGetUniformLocation(program, name);
        this->uniformSet[key] = false;
    }
}

void ShaderProgram::set_textures(){
    for(auto itr = textureIds.begin(); itr!=textureIds.end(); ++itr){
        auto i = (int)std::distance(textureIds.begin(), itr);
        glActiveTexture((GLenum)(GL_TEXTURE0+i));
        glBindTexture(GL_TEXTURE_2D, itr->second);
        if(generateMipmap[itr->second])glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout<<"Bind texture "<<itr->second<<" to GL_TEXTURE"<<i<<std::endl;
    }
}

void ShaderProgram::setUniform(const char* uni, GLboolean val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform1i(uniforms[uni], val);};
void ShaderProgram::setUniform(const char* uni, GLfloat val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform1f(uniforms[uni], val);};
void ShaderProgram::setUniform(const char* uni, GLdouble val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform1d(uniforms[uni], val);};
void ShaderProgram::setUniform(const char* uni, GLuint val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform1ui(uniforms[uni], val);};
void ShaderProgram::setUniform(const char* uni, const glm::vec2& val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform2fv(uniforms[uni], 1, glm::value_ptr(val));};
void ShaderProgram::setUniform(const char* uni, const glm::vec3& val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform3fv(uniforms[uni], 1, glm::value_ptr(val));};
void ShaderProgram::setUniform(const char* uni, const float val[3]){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform3fv(uniforms[uni], 1, val);};
void ShaderProgram::setUniform(const char* uni, const std::vector<glm::vec3>& val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniform3fv(uniforms[uni], val.size(), glm::value_ptr(val.front()));};
void ShaderProgram::setUniform(const char* uni, const glm::mat4& val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniformMatrix4fv(uniforms[uni], 1, GL_FALSE, glm::value_ptr(val));};
void ShaderProgram::setUniform(const char* uni, const glm::mat3& val){glUseProgram(program);if(uniforms.count(uni)==0)fprintf(stderr,"Uniform Variable %s Not Found\n", uni);uniformSet[uni]=true;glUniformMatrix3fv(uniforms[uni], 1, GL_FALSE, glm::value_ptr(val));};
void ShaderProgram::setUniform(const char* uni, Texture* val){
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
void ShaderProgram::setUniform(const char* uni, FrameBuffer* val){this->setUniform(uni, val->textures[0]);};

void ShaderProgram::setShaderStorageBuffer(const char* name, ShaderStorageBuffer* buffer){
    GLuint index = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, name);
    glShaderStorageBlockBinding(program, index, buffer->storage_block_binding);
}