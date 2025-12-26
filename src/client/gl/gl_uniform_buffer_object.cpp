#include <QOpenGLVersionFunctionsFactory>

#include "gl_uniform_buffer_object.h"

#define GL (*gl::g_glFunctions)

namespace gl {
  OpenGLUniformBufferObject::OpenGLUniformBufferObject() {
    m_id = 0;
  }

  OpenGLUniformBufferObject::~OpenGLUniformBufferObject() {
    if (m_id)
      destroy();
  }

  GLuint OpenGLUniformBufferObject::bufferId() const { return m_id; }

  bool OpenGLUniformBufferObject::create() {
    if (m_id)
      destroy();
    GL.glGenBuffers(1, &m_id);
    return m_id > 0;
  }

  void OpenGLUniformBufferObject::allocate(int indx, const void *data, int count) {
    if (m_id == 0)
      return;
    GL.glBufferData(GL_UNIFORM_BUFFER, count, data, GL_STATIC_DRAW);
    GL.glBindBufferRange(GL_UNIFORM_BUFFER, indx, m_id, 0, count);
  }

  void OpenGLUniformBufferObject::destroy() {
    if (m_id) {
      GL.glDeleteBuffers(1, &m_id);
      m_id = 0;
    }
  }

  void OpenGLUniformBufferObject::bind() {
    if (m_id)
      GL.glBindBuffer(GL_UNIFORM_BUFFER, m_id);
  }

  void OpenGLUniformBufferObject::write(int offset, const void *data, int count) {
    if (m_id)
      GL.glBufferSubData(GL_UNIFORM_BUFFER, offset, count, data);
  }

  void OpenGLUniformBufferObject::release() {
    GL.glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void OpenGLUniformBufferObject::bindUniformBlock(QOpenGLShaderProgram *shader) {
    // GLuint indx = GL.glGetUniformBlockIndex(shader->programId(), "CameraInfo");
    // GL.glUniformBlockBinding(shader->programId(), indx, CAMERA_INFO_BINDING_POINT);
    // indx = GL.glGetUniformBlockIndex(shader->programId(), "ModelInfo");
    // GL.glUniformBlockBinding(shader->programId(), indx, MODEL_INFO_BINDING_POINT);
    // indx = GL.glGetUniformBlockIndex(shader->programId(), "MaterialInfo");
    // GL.glUniformBlockBinding(shader->programId(), indx, MATERIAL_INFO_BINDING_POINT);
    GLuint indx = GL.glGetUniformBlockIndex(shader->programId(), "LightInfo");
    GL.glUniformBlockBinding(shader->programId(), indx, LIGHT_INFO_BINDING_POINT);
  }
}