#pragma once
#include "texture.h"
#include "texture_image.h"

inline void TextureImage::bind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_albedoID);
}

inline void TextureImage::unbind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);  // @TODO: 0?
}