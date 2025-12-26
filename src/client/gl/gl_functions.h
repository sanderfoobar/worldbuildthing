#pragma once
#include <GL/gl.h>
#include <QOpenGLFunctions_4_5_Core>
#include <memory>
#include <string>
#include "engine/mesh.h"

namespace gl {

// Global OpenGL functions context
extern QOpenGLFunctions_4_5_Core* g_glFunctions;

// Legacy triangle functions
extern GLuint createTriangle(float _size);
extern GLuint loadShaderFromStrings(const std::string &_vertex, const std::string &_fragment);
extern GLuint loadShaderProgramFromDisk(const std::string &path_vertex, const std::string &path_frag);

// Cube and mesh creation
extern std::shared_ptr<Mesh> createCubeMesh(float size = 1.0f);
extern GLuint loadShaderProgram(const std::string &_vertex, const std::string &_fragment);

}  // namespace gl
