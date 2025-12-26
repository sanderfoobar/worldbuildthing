#include "texture_loader.h"
#include <cstring>
#include <iostream>
#include "gl_functions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "shared/lib/stb/stb_image.h"

#define GL (*gl::g_glFunctions)

namespace gl::texdb {

std::map<QString, GLuint> lookup = {};

void unload(const GLuint tex_id) {
  if (tex_id == 0)
    return;
  GL.glDeleteTextures(1, &tex_id);
}

void unload(const QString &name) {
  if (lookup.contains(name))
    unload(lookup[name]);
}

GLuint upload(const QString &name, const unsigned char* data, const int width, const int height, const int nrChannels, const TextureType type) {
  GLuint tex_id;
  GL.glGenTextures(1, &tex_id);

  GLenum internalFormat = GL_RGB8;
  GLenum dataFormat = GL_RGB;

  switch (type) {
    case TextureType::Albedo:
      internalFormat = (nrChannels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
      dataFormat     = (nrChannels == 4) ? GL_RGBA : GL_RGB;
      break;
    case TextureType::Normal:
      internalFormat = (nrChannels == 3) ? GL_RGB8 : GL_R8;
      dataFormat     = (nrChannels == 3) ? GL_RGB : GL_RED;
      break;
    case TextureType::Metallic:
    case TextureType::Roughness:
    case TextureType::AO:
    case TextureType::Specular:
      internalFormat = GL_R8;
      dataFormat     = GL_RED;
      break;
    case TextureType::ARM:
      internalFormat = GL_RGB8;
      dataFormat     = GL_RGB;
      break;
  }

  GL.glBindTexture(GL_TEXTURE_2D, tex_id);
  GL.glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
  GL.glGenerateMipmap(GL_TEXTURE_2D);

  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (type == TextureType::Albedo) {
    GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  } else {
    GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  lookup[name] = tex_id;
  return tex_id;
}

GLuint createTextureFromDisk(const QString &name, const std::string& filepath, const TextureType type) {
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture: " << filepath << " | " << stbi_failure_reason() << std::endl;
    return 0;
  }

  const GLuint textureID = upload(name, data, width, height, nrChannels, type);
  stbi_image_free(data);

  std::cerr << "Texture loaded: " << filepath << " (" << width << "x" << height << ", channels: " << nrChannels << ")" << std::endl;
  return textureID;
}

GLuint createTextureFromMemory(const QString &name, const unsigned char* buffer, const size_t size, const TextureType type) {
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load_from_memory(buffer, static_cast<int>(size), &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Failed to load texture from memory | " << stbi_failure_reason() << std::endl;
    return 0;
  }

  const GLuint textureID = upload(name, data, width, height, nrChannels, type);
  stbi_image_free(data);

  std::cerr << "Texture loaded from memory (" << width << "x" << height << ", channels: " << nrChannels << ")" << std::endl;
  return textureID;
}

GLuint createSolidColorTexture(const float r, const float g, const float b) {
  const unsigned char pixels[3] = {
    static_cast<unsigned char>(r * 255.0f),
    static_cast<unsigned char>(g * 255.0f),
    static_cast<unsigned char>(b * 255.0f)
  };

  GLuint textureID;
  GL.glGenTextures(1, &textureID);
  GL.glBindTexture(GL_TEXTURE_2D, textureID);
  GL.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  GL.glGenerateMipmap(GL_TEXTURE_2D);

  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureID;
}

}
