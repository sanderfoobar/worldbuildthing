#include <QBuffer>
#include <QCoreApplication>
#include <QDirIterator>
#include <QObject>
#include <QResource>

#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "stb.h"

#include "shared/models/texture.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "shared/lib/stb/stb_image.h"

namespace stb {
  bool generate_stb_files(void* tex) {
    // auto *texture = static_cast<Texture *>(tex);
    // qDebug() << "generate_vmt_vtf_files" << texture->name;
    //
    // const auto diffuse = texture->get_diffuse(TextureSize::x1024, true);
    // QFileInfo path_input_image = diffuse->path;
    // QFileInfo path_thumb = diffuse->path_thumbnail();
    // QString asset_pack_name = texture->asset_pack()->name();
    // QString texture_name = diffuse->name;
    //
    // qDebug() << path_thumb.absoluteFilePath();
    // // int width, height, nrChannels;
    // // unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
    //
    // auto aa = path_input_image.absoluteFilePath().toStdString();
    // int width, height, nrChannels;
    // unsigned char* data = stbi_load(aa.c_str(), &width, &height, &nrChannels, 0);
    //
    // QByteArray bytes = QByteArray(data);
    // Utils::fileWrite("/tmp/lol.test", bytes);
    // int wgiwoe = 1;
    //

    return true;
  }
}