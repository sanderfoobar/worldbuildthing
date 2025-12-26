#include "texture_manager.h"

#include "client/lib/globals.h"
#include "shared/lib/stb/stb_image.h"
#include "client/gl/gl_functions.h"

namespace gl {
  #define GL (*gl::g_glFunctions)

  GLTextureManager::GLTextureManager() {
    qDebug() << "TextureManager constructor called";
  }

  void GLTextureManager::uploadTexture(QSharedPointer<TextureImage> img) {
    // pack->
  }

  QSharedPointer<Material> GLTextureManager::get_by_name(const QString &name) {
    if (m_entries_by_name.contains(name))
      return m_entries_by_name[name];
    return m_entries_by_name["devtexture01"];
  }

  GLuint GLTextureManager::load_texture_from_memory(const QByteArray& data) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* img = stbi_load_from_memory(
      reinterpret_cast<const unsigned char*>(data.constData()),
      data.size(), &width, &height, &channels, 0
    );

    if (!img) {
      qCritical() << "Failed to load texture from memory";
      return 0;
    }

    GLuint tex;
    GL.glGenTextures(1, &tex);
    GL.glBindTexture(GL_TEXTURE_2D, tex);
    GL.glTexImage2D(GL_TEXTURE_2D, 0, (channels == 4 ? GL_RGBA : GL_RGB),
                 width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB),
                 GL_UNSIGNED_BYTE, img);
    GL.glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(img);
    return tex;
  }

  void GLTextureManager::init() {
    // setup fallback textures
    auto create1x1Texture = [this](unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) -> GLuint {
      GLuint tex;
      GL.glGenTextures(1, &tex);
      GL.glBindTexture(GL_TEXTURE_2D, tex);
      const unsigned char data[4] = {r, g, b, a};
      GL.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      return tex;
    };

    g::texDefaultGray = create1x1Texture(128, 128, 128);
    g::texDefaultWhite = create1x1Texture(255, 255, 255);
    g::texDefaultBlack = create1x1Texture(0, 0, 0);
    g::texDefaultNormal = create1x1Texture(128, 128, 255);

    // dev texture
    QFile file(":/assets/measurewall01_Color.jpg");
    QByteArray tex_devtexture;
    if (file.open(QIODevice::ReadOnly))
      tex_devtexture = file.readAll();

    const GLuint tex_devtexture_id = load_texture_from_memory(tex_devtexture);
    const auto tex_devtexture_ptr = QSharedPointer<Material>(new Material(tex_devtexture_id));
    m_entries_by_name["devtexture01"] = tex_devtexture_ptr;

    m_initialized = true;
  }
}