#include "glcpp.hpp"
#include <opencv2/opencv.hpp>

using namespace glcpp;
 
GLuint Texture::getTextureId(){
        return texture_id;
}

 Texture::Texture(const char* file, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLint type){
    cv::Mat image = cv::imread(file);
    if(image.empty())std::cerr<<"Image \""<<file<<"\" not found"<<std::endl;
    width = image.cols;
    height = image.rows;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    if(minFilter==GL_LINEAR_MIPMAP_LINEAR||minFilter==GL_LINEAR_MIPMAP_NEAREST||minFilter==GL_NEAREST_MIPMAP_LINEAR||minFilter==GL_NEAREST_MIPMAP_NEAREST)
        generateMipmap = true;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, // target
                    0, // level, 0 = base, no minimap,
                    internalFormat, // internalformat
                    image.cols, // width
                    image.rows, // height
                    0, // border
                    GL_BGR, // format
                    type, // type
                    image.ptr());
    image.release();
}
Texture::Texture(cv::Mat image, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLint type){
    if(image.empty())std::cerr<<"Image not loaded";
    width = image.cols;
    height = image.rows;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    if(minFilter==GL_LINEAR_MIPMAP_LINEAR||minFilter==GL_LINEAR_MIPMAP_NEAREST||minFilter==GL_NEAREST_MIPMAP_LINEAR||minFilter==GL_NEAREST_MIPMAP_NEAREST)
        generateMipmap = true;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, // target
                    0, // level, 0 = base, no minimap,
                    internalFormat, // internalformat
                    image.cols, // width
                    image.rows, // height
                    0, // border
                    GL_BGR, // format
                    type, // type
                    image.ptr());
}
Texture::Texture(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format){
    this->width = width; this->height = height;

    glGenTextures(1, &texture_id);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    if(minFilter==GL_LINEAR_MIPMAP_LINEAR||minFilter==GL_LINEAR_MIPMAP_NEAREST||minFilter==GL_NEAREST_MIPMAP_LINEAR||minFilter==GL_NEAREST_MIPMAP_NEAREST)
        generateMipmap = true;
}
Texture::Texture(GLfloat data[],GLsizei width, GLsizei height, GLint minFilter,
        GLint magFilter, GLint wrapS, GLint wrapT,
        GLint internalFormat, GLenum format, GLenum type){
    this->width = width; this->height = height;

    glGenTextures(1, &texture_id);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, // target
                    0, // level, 0 = base, no minimap,
                    internalFormat, // internalformat
                    width, // width
                    height, // height
                    0, // border, always 0 in OpenGL ES
                    format, // format
                    type, // type
                    data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}
Texture::~Texture(){
    glDeleteTextures(1, &texture_id);
}
void Texture::getTexImage(GLvoid* pixels){
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glGetTexImage(GL_TEXTURE_2D, 0,GL_BGR, GL_UNSIGNED_BYTE, pixels);
}