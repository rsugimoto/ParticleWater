#ifndef __GL_CPP__
#define __GL_CPP__

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <GLUT/GLUT.h>
#else
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glut.h>
#endif

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <limits.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/string_cast.hpp"

namespace cv{
    class Mat;
}

namespace glcpp{

class BufferGeometry{
protected:
    GLuint vbo, ibo;
    void render(GLuint program);
public:
    ~BufferGeometry();
    BufferGeometry();
    friend class Renderer;
};

class PlaneBufferGeometry : public BufferGeometry{
public:
    PlaneBufferGeometry(int width, int height, int widthSegments=1, int heightSegments=1);
};

class ScreenPlaneBufferGeometry: public BufferGeometry{
public:
    ScreenPlaneBufferGeometry(int screenWidth,int screenHeight,int geometryDensity);
};

class Texture{
private:
    GLuint texture_id;
    GLuint getTextureId();
public:
    int width, height;
    bool generateMipmap = false;
    //Constructor for making a texture from an image file.
    Texture(const char* file, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLint type=GL_UNSIGNED_BYTE);
    Texture(cv::Mat image, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLint type=GL_UNSIGNED_BYTE);
    Texture(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format);
    Texture(GLfloat data[],GLsizei width, GLsizei height, GLint minFilter= GL_NEAREST,//GL_NEAREST_MIPMAP_LINEAR,
            GLint magFilter=GL_NEAREST, GLint wrapS=GL_CLAMP_TO_EDGE, GLint wrapT=GL_CLAMP_TO_EDGE,
            GLint internalFormat = GL_RGBA32F, GLenum format = GL_RGBA, GLenum type=GL_FLOAT);
    ~Texture();
    void getTexImage(GLvoid* pixels);

    friend class FrameBuffer;
    friend class ShaderMaterial;
};

class FrameBuffer{
private:
    GLuint frameBufferId = 0;

public:
    std::vector<Texture*> textures;
    GLuint depthrenderbuffer = UINT_MAX;
    FrameBuffer(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format, bool enableDepth=false);
    ~FrameBuffer();
    void attachDepthBuffer();
    void attachAdditionalBuffer(GLint internalFormat = GL_RGBA32F, GLenum format = GL_RGBA);
    void attachAdditionalBuffer(GLsizei width, GLsizei height, GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT, GLint internalFormat, GLenum format);
    void setRenderTarget();
};

class ShaderMaterial{
private:
    GLuint program;
    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, bool> uniformSet;
    std::map<std::string, GLuint> textureIds;
    std::unordered_map<GLuint, bool> generateMipmap;

    char* file_read(const char* filename);
    void print_log(GLuint object);
    GLuint create_shader(const char* filename, GLenum type);
    GLuint create_program(const char *vert, const char *frag);
    void render();
    GLuint getProgramId();
public:
    bool blending = true;
    bool depthTest = true;
    bool wireFrame = false;
    ShaderMaterial(const char* vert, const char* frag);
    ~ShaderMaterial();
    void setUniform(const char* uni, GLboolean val);
    void setUniform(const char* uni, GLfloat val);
    void setUniform(const char* uni, const glm::vec2& val);
    void setUniform(const char* uni, const glm::vec3& val);
    void setUniform(const char* uni, const float val[3]);
    void setUniform(const char* uni, const glm::mat4& val);
    void setUniform(const char* uni, const glm::mat3& val);
    void setUniform(const char* uni, Texture* val);
    void setUniform(const char* uni, FrameBuffer* val);
    void setUniform(const char* uni, const std::vector<glm::vec3>& val);
    friend class Renderer;
};


class Renderer{
private:
    int width, height;
public:
    Renderer(int width, int height);
    void render(BufferGeometry* geometry, ShaderMaterial* material, FrameBuffer* renderTarget = nullptr);
};

}
#endif //__GL_CPP__