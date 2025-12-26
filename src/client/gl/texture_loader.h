#pragma once

#include <GL/gl.h>
#include <QString>
#include <map>
#include <qmetatype.h>
#include <string>

namespace gl::texdb {

extern std::map<QString, GLuint> lookup;

enum class TextureType {
  Albedo,
  Normal,
  Metallic,
  Roughness,
  AO,
  ARM,
  Specular
};

void unload(GLuint tex_id);
void unload(const QString &name);
GLuint upload(const QString &name, const unsigned char *data, int width, int height, int nrChannels, TextureType type);
GLuint createTextureFromDisk(const QString &name, const std::string &filepath, TextureType type);
GLuint createTextureFromMemory(const QString &name, const unsigned char *buffer, size_t size, TextureType type);
GLuint createSolidColorTexture(float r, float g, float b);

}
