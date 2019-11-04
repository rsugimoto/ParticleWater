#include "glcpp.hpp"
#include <math.h>
#include <vector>
#include <cstdio>

using namespace glcpp;

BufferGeometry::BufferGeometry() {
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
}

BufferGeometry::~BufferGeometry(){
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void BufferGeometry::render(GLuint program){
    const char* attribute_name;
    attribute_name = "position";
    glUseProgram(program);
    GLint attribute_location = glGetAttribLocation(program, attribute_name);
    if (attribute_location == -1) fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    else{
        glEnableVertexAttribArray((GLuint)attribute_location);
        // Describe our vertices array to OpenGL (it can't guess its format automatically)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(
                (GLuint)attribute_location, // attribute
                3,                 // number of elements per vertex, here (x,y,z)
                GL_FLOAT,          // the type of each element
                GL_FALSE,          // take our values as-is
                0,                 // no extra data between each position
                0                  // offset of first element
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        int size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray((GLuint)attribute_location);
    }
}

PlaneBufferGeometry::PlaneBufferGeometry(int width, int height, int widthSegments, int heightSegments) : BufferGeometry(){
    GLfloat width_half = (GLfloat)width / 2;
    GLfloat height_half = (GLfloat)height / 2;

    int gridX = widthSegments;
    int gridY = heightSegments;

    int gridX1 = gridX + 1;
    int gridY1 = gridY + 1;

    GLfloat segment_width = (GLfloat)width / gridX;
    GLfloat segment_height = (GLfloat)height / gridY;

    int ix, iy;
    auto vertices = new GLfloat[gridY1*gridX1*3];
    auto indices = new GLuint[gridY*gridX*6];

    // generate vertices, normals and uvs
    for ( iy = 0; iy < gridY1; iy ++ ) {
        GLfloat y = iy * segment_height - height_half;
        for ( ix = 0; ix < gridX1; ix ++ ) {
            GLfloat x = ix * segment_width - width_half;

            vertices[iy*gridX1*3 + ix*3] = x;
            vertices[iy*gridX1*3 + ix*3 + 1] = -y;
            vertices[iy*gridX1*3 + ix*3 + 2] = 0;
        }
    }

    // indices
    for ( iy = 0; iy < gridY; iy ++ ) {
        for ( ix = 0; ix < gridX; ix ++ ) {

            GLuint a = (GLuint)(ix + gridX1 * iy);
            GLuint b = (GLuint)(ix + gridX1 * ( iy + 1 ));
            GLuint c = (GLuint)(( ix + 1 ) + gridX1 * ( iy + 1 ));
            GLuint d = (GLuint)(( ix + 1 ) + gridX1 * iy);

            indices[iy*gridX*6 + ix*6] = a;
            indices[iy*gridX*6 + ix*6 + 1] = b;
            indices[iy*gridX*6 + ix*6 + 2] = d;

            indices[iy*gridX*6 + ix*6 + 3] = b;
            indices[iy*gridX*6 + ix*6 + 4] = c;
            indices[iy*gridX*6 + ix*6 + 5] = d;

        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (gridY1*gridX1*3), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (gridY*gridX*6), indices, GL_STATIC_DRAW);

    delete[] vertices;
    delete[] indices;
}

ScreenPlaneBufferGeometry::ScreenPlaneBufferGeometry(int screenWidth,int screenHeight,int geometryDensity) : BufferGeometry(){
    long gridX = (long)ceil(screenWidth*8.0 / geometryDensity);
    long gridY = (long)ceil(screenHeight*8.0 / geometryDensity);

    long gridX1 = gridX + 1;
    long gridY1 = gridY + 1;

    long ix, iy;
    auto vertices = new GLfloat[gridY1*gridX1*3];
    auto indices = new GLuint[gridY*gridX*6];

    // generate vertices
    for ( iy = 0; iy < gridY1; iy ++ ) {

        GLfloat y = 8.0f*iy/gridY - 4.0f;

        for ( ix = 0; ix < gridX1; ix ++ ) {

            GLfloat x = 8.0f*ix/gridX - 4.0f;

            vertices[iy*gridX1*3 + ix*3] = x;
            vertices[iy*gridX1*3 + ix*3 + 1] = -y;
            vertices[iy*gridX1*3 + ix*3 + 2] = 0;
        }
    }

    // indices
    for ( iy = 0; iy < gridY; iy ++ ) {

        for ( ix = 0; ix < gridX; ix ++ ) {

            GLuint a = (GLuint)(ix + gridX1 * iy);
            GLuint b = (GLuint)(ix + gridX1 * ( iy + 1 ));
            GLuint c = (GLuint)(( ix + 1 ) + gridX1 * ( iy + 1 ));
            GLuint d = (GLuint)(( ix + 1 ) + gridX1 * iy);

            indices[iy*gridX*6 + ix*6] = a;
            indices[iy*gridX*6 + ix*6 + 1] = b;
            indices[iy*gridX*6 + ix*6 + 2] = d;

            indices[iy*gridX*6 + ix*6 + 3] = b;
            indices[iy*gridX*6 + ix*6 + 4] = c;
            indices[iy*gridX*6 + ix*6 + 5] = d;
        }

    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (gridY1*gridX1*3), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (gridY*gridX*6), indices, GL_STATIC_DRAW);

    delete[] vertices;
    delete[] indices;
}
