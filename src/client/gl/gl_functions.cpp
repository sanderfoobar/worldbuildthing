#include <memory>
#include <array>
#include <iostream>

#include <glm/glm.hpp>
#include "gl/gl_functions.h"

#include "shared/lib/utils.h"
#include "engine/mesh.h"

namespace gl {

// Global OpenGL functions pointer
QOpenGLFunctions_4_5_Core* g_glFunctions = nullptr;

// Macro for easy access
#define GL (*g_glFunctions)

GLuint createTriangle(const float _size) {
  GLuint vaoID;
  // allocate a VertexArray
  GL.glGenVertexArrays(1, &vaoID);
  // now bind a vertex array object for our verts
  GL.glBindVertexArray(vaoID);
  // a simple triangle
  std::array<float, 9> vert; // vertex array
  vert[0] = -_size;
  vert[1] = -_size;
  vert[2] = 0.0f;
  vert[3] = 0;
  vert[4] = _size;
  vert[5] = 0.0f;
  vert[6] = _size;
  vert[7] = -_size;
  vert[8] = 0.0f;
  // now we are going to bind this to our vbo
  GLuint vboID;
  GL.glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  GL.glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer data
  GL.glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert[0], GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  GL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  GL.glEnableVertexAttribArray(0);

  // Now for the colour

  std::array<float, 9> colour = {1.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f};

  GLuint colourvboID;
  GL.glGenBuffers(1, &colourvboID);
  // now bind this to the VBO buffer
  GL.glBindBuffer(GL_ARRAY_BUFFER, colourvboID);
  // allocate the buffer data
  GL.glBufferData(GL_ARRAY_BUFFER, colour.size() * sizeof(float), &colour[0], GL_STATIC_DRAW);
  // now fix this to the attribute buffer 1
  GL.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inColour in the shader)
  GL.glEnableVertexAttribArray(1);
  // this basically switches off the current Vertex array object
  GL.glBindVertexArray(0);
  return vaoID;
}

void printInfoLog(const GLuint &_obj, GLenum _mode = GL_COMPILE_STATUS) {
  GLint infologLength = 0;
  GLint charsWritten = 0;
  std::unique_ptr<char []> infoLog;

  GL.glGetShaderiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);
  std::cerr << "info log length " << infologLength << "\n";
  if (infologLength > 0) {
    infoLog = std::make_unique<char []>(infologLength);
    GL.glGetShaderInfoLog(_obj, infologLength, &charsWritten, &infoLog[0]);

    std::cerr << infoLog.get() << '\n';
    GL.glGetShaderiv(_obj, _mode, &infologLength);
    if (infologLength == GL_FALSE) {
      if (_mode == GL_COMPILE_STATUS)
        std::cerr << "Shader compile failed or had warnings \n";
      else
        std::cerr << "linker error\n";
      exit(EXIT_FAILURE);
    }
  }
}

GLuint loadShaderFromStrings(const std::string &_vertex, const std::string &_fragment) {
  // here we create a program
  GLuint shaderID = GL.glCreateProgram();

  // create a Vertex shader object
  GLuint vertexID = GL.glCreateShader(GL_VERTEX_SHADER);
  // attatch the shader source we need to convert to GL format
  const char *source = _vertex.c_str();
  GL.glShaderSource(vertexID, 1, &source,NULL);
  // now compile the shader
  GL.glCompileShader(vertexID);
  std::cerr << "compiling vertex shader\n";
  printInfoLog(vertexID);

  // now create a fragment shader
  GLuint fragmentID = GL.glCreateShader(GL_FRAGMENT_SHADER);
  // attatch the shader source
  source = _fragment.c_str();
  GL.glShaderSource(fragmentID, 1, &source,NULL);
  // compile the shader
  std::cerr << "compiling frag shader shader\n";
  GL.glCompileShader(fragmentID);
  printInfoLog(fragmentID);
  // now attach to the program object
  GL.glAttachShader(shaderID, vertexID);
  GL.glAttachShader(shaderID, fragmentID);

  // link the program
  GL.glLinkProgram(shaderID);
  printInfoLog(shaderID,GL_LINK_STATUS);
  // and enable it for use
  GL.glUseProgram(shaderID);
  // now tidy up the shaders as we don't need them
  GL.glDeleteShader(vertexID);
  GL.glDeleteShader(fragmentID);
  return shaderID;
}

GLuint loadShaderProgramFromDisk(const std::string &path_vertex, const std::string &path_frag) {
  const auto vert = Utils::fileOpen(QString::fromStdString(path_vertex));
  const auto frag = Utils::fileOpen(QString::fromStdString(path_frag));
  return loadShaderFromStrings(vert.toStdString(), frag.toStdString());
}

GLuint loadShaderProgram(const std::string &_vertex, const std::string &_fragment) {
  return loadShaderFromStrings(_vertex, _fragment);
}

}  // namespace gl
