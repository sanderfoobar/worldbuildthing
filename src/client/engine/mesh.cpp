#include "mesh.h"
#include <iostream>
#include "../gl/gl_functions.h"
#include "client/lib/globals.h"
#include "shared/lib/globals.h"

// Macro to access OpenGL functions
#define GL (*gl::g_glFunctions)

namespace gl {
  static glm::vec3 qv_to_glm(const QVector3D &q) {
    return glm::vec3(q.x(), q.y(), q.z());
  }

  // std::shared_ptr<Mesh> Mesh::createFromCubeSides(const std::array<Side, 6> &sides) {
  //   auto mesh = std::make_shared<Mesh>();
  //   std::vector<Vertex> vertices;
  //   std::vector<GLuint> indices;
  //   GLuint idxOffset = 0;
  //
  //   const bool flipU = false;
  //   const bool flipV = true;
  //   const bool swapUV = true;
  //   const bool invertWinding = true;
  //
  //   glm::vec3 faceNormals[6] = {
  //     { 1, 0, 0}, {-1, 0, 0},
  //     { 0, 1, 0}, { 0,-1, 0},
  //     { 0, 0, 1}, { 0, 0,-1}
  //   };
  //
  //   for (int face = 0; face < 6; ++face) {
  //     const auto &s = sides[face];
  //     glm::vec3 uVec(s.uaxis[0], s.uaxis[1], s.uaxis[2]);
  //     glm::vec3 vVec(s.vaxis[0], s.vaxis[1], s.vaxis[2]);
  //     float u_offset = s.uaxis[3];
  //     float v_offset = s.vaxis[3];
  //     float u_scale = s.uaxis[4];
  //     float v_scale = s.vaxis[4];
  //
  //     if (u_scale < 0.25f) u_scale /= 0.25f;
  //     if (v_scale < 0.25f) v_scale /= 0.25f;
  //
  //     float horizontal_uv_offset = 0.0f;
  //     glm::vec3 normal = faceNormals[face];
  //
  //     for (int i = 0; i < 4; ++i) {
  //       glm::vec3 pos(s.vertices_plus[i].x(), s.vertices_plus[i].y(), s.vertices_plus[i].z());
  //       float u = (glm::dot(pos, uVec) + u_offset + horizontal_uv_offset) * u_scale;
  //       float v = (glm::dot(pos, vVec) + v_offset) * v_scale;
  //       if (swapUV) std::swap(u,v);
  //       if (flipU) u = -u;
  //       if (flipV) v = -v;
  //
  //       vertices.push_back(Vertex{pos, normal, glm::vec2(u,v)});
  //     }
  //
  //     if (!invertWinding) {
  //       indices.insert(indices.end(), {idxOffset, idxOffset+1, idxOffset+2, idxOffset, idxOffset+2, idxOffset+3});
  //     } else {
  //       indices.insert(indices.end(), {idxOffset, idxOffset+2, idxOffset+1, idxOffset, idxOffset+3, idxOffset+2});
  //     }
  //
  //     idxOffset += 4;
  //   }
  //
  //   mesh->addVertices(vertices);
  //   mesh->addIndices(indices);
  //   return mesh;
  // }

  std::shared_ptr<Mesh> Mesh::createFromCubeSides(const std::array<Side, 6> &sides) {
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint idxOffset = 0;

    const bool flipU = false; // flip U coordinate
    const bool flipV = true; // flip V coordinate
    const bool swapUV = true; // swap U and V (rotate 90°)
    const bool invertWinding = false; // reverse triangle order

    for (int face = 0; face < 6; ++face) {
      const auto &s = sides[face];

      glm::vec3 uVec(s.uaxis[0], s.uaxis[1], s.uaxis[2]);
      glm::vec3 vVec(s.vaxis[0], s.vaxis[1], s.vaxis[2]);
      float u_offset = s.uaxis[3];
      float v_offset = s.vaxis[3];
      float u_scale = s.uaxis[4];
      float v_scale = s.vaxis[4];

      if (u_scale < 0.25f)
        u_scale /= 0.25f;
      if (v_scale < 0.25f)
        v_scale /= 0.25f;

      float horizontal_uv_offset = 0.0f;

      glm::vec3 normal = glm::normalize(glm::cross(
          glm::vec3(s.vertices_plus[1].x() - s.vertices_plus[0].x(),
                    s.vertices_plus[1].y() - s.vertices_plus[0].y(),
                    s.vertices_plus[1].z() - s.vertices_plus[0].z()),
          glm::vec3(s.vertices_plus[2].x() - s.vertices_plus[0].x(),
                    s.vertices_plus[2].y() - s.vertices_plus[0].y(),
                    s.vertices_plus[2].z() - s.vertices_plus[0].z())
          ));

      for (int i = 0; i < 4; ++i) {
        glm::vec3 pos(s.vertices_plus[i].x(), s.vertices_plus[i].y(), s.vertices_plus[i].z());

        float u = (glm::dot(pos, uVec) + u_offset + horizontal_uv_offset) * u_scale;
        float v = (glm::dot(pos, vVec) + v_offset) * v_scale;

        // apply test flags
        if (swapUV)
          std::swap(u, v);
        if (flipU)
          u = -u;
        if (flipV)
          v = -v;

        vertices.push_back(Vertex{pos, normal, glm::vec2(u, v)});
      }

      if (!invertWinding) {
        indices.insert(indices.end(), {idxOffset, idxOffset + 1, idxOffset + 2,
                                       idxOffset, idxOffset + 2, idxOffset + 3});
      } else {
        indices.insert(indices.end(), {idxOffset, idxOffset + 2, idxOffset + 1,
                                       idxOffset, idxOffset + 3, idxOffset + 2});
      }

      idxOffset += 4;
    }

    mesh->addVertices(vertices);
    mesh->addIndices(indices);
    return mesh;
  }

  Mesh::Mesh() :
    m_vao(0), m_vbo(0), m_ebo(0), m_isDirty(false) {
    // Initialize with 6 default white materials (one per face)
    for (int i = 0; i < 6; ++i) {
      m_faceMaterials.push_back(g::glTextureManager->get_by_name("devtexture01"));
      m_faceUVScales.push_back(glm::vec2(1.0f, 1.0f));
      m_faceUVOffsets.push_back(glm::vec2(0.0f, 0.0f));
    }

    m_faceMaterials.reserve(7);
    m_faceUVScales.reserve(7);
    m_faceUVOffsets.reserve(7);
  }

  void Mesh::setFaceUVScale(int faceIndex, const glm::vec2 &scale) {
    if (faceIndex >= 0 && faceIndex < (int) m_faceUVScales.size())
      m_faceUVScales[faceIndex] = scale;
  }

  void Mesh::setFaceUVOffset(int faceIndex, const glm::vec2 &offset) {
    if (faceIndex >= 0 && faceIndex < (int) m_faceUVOffsets.size())
      m_faceUVOffsets[faceIndex] = offset;
  }

  glm::vec2 Mesh::getFaceUVScale(int faceIndex) const {
    if (faceIndex >= 0 && faceIndex < (int) m_faceUVScales.size())
      return m_faceUVScales[faceIndex];
    return glm::vec2(1.0f, 1.0f);
  }

  glm::vec2 Mesh::getFaceUVOffset(int faceIndex) const {
    if (faceIndex >= 0 && faceIndex < (int) m_faceUVOffsets.size())
      return m_faceUVOffsets[faceIndex];
    return glm::vec2(0.0f, 0.0f);
  }

  void Mesh::addVertices(const std::vector<Vertex> &vertices) {
    // IMPORTANT: do not clamp VMF-derived UVs here. VMF uses world-space axes;
    // per-face UV scale/offset calls should control final sampling. Keeping raw UVs
    // preserves correct tiling and offsets.
    m_vertices = vertices;
    m_isDirty = true;

    // debug print added vertices
    // for (size_t i = 0; i < vertices.size(); ++i) {
    //   const auto &v = vertices[i];
    //   qDebug().nospace()
    //     << "Vertex " << i << ": "
    //     << "pos=(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ") "
    //     << "normal=(" << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << ") "
    //     << "uv=(" << v.texCoord.x << ", " << v.texCoord.y << ")";
    // }
  }

  void Mesh::addIndices(const std::vector<GLuint> &indices) {
    m_indices = indices;
    m_isDirty = true;
  }

  void Mesh::add_face_material(int faceIndex, const QSharedPointer<Material> &material) {
    if (faceIndex >= 0) {
      if (faceIndex >= (int) m_faceMaterials.size()) {
        // grow arrays to accommodate more faces; each face consumes 6 indices.
        int needed = faceIndex + 1 - (int) m_faceMaterials.size();
        for (int i = 0; i < needed; ++i) {
          m_faceMaterials.push_back(g::glTextureManager->get_by_name("devtexture01"));
          m_faceUVScales.push_back(glm::vec2(1.0f, 1.0f));
          m_faceUVOffsets.push_back(glm::vec2(0.0f, 0.0f));
        }
      }
      m_faceMaterials[faceIndex] = material;
    }
  }

  void Mesh::clearGeometry() {
    m_vertices.clear();
    m_indices.clear();
    m_isDirty = true;
  }

  void Mesh::setupBuffers() {
    if (m_vao == 0) {
      GL.glGenVertexArrays(1, &m_vao);
    }

    GL.glBindVertexArray(m_vao);

    // VBO for vertex data
    if (m_vbo == 0) {
      GL.glGenBuffers(1, &m_vbo);
    }
    GL.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    GL.glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    // EBO for indices
    if (m_ebo == 0) {
      GL.glGenBuffers(1, &m_ebo);
    }
    GL.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    GL.glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    // Position (location 0)
    GL.glEnableVertexAttribArray(0);
    GL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, position));

    // Normal (location 1)
    GL.glEnableVertexAttribArray(1);
    GL.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

    // TexCoord (location 2)
    GL.glEnableVertexAttribArray(2);
    GL.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, texCoord));

    // No tangent/bitangent attributes (normal mapping removed)

    GL.glBindVertexArray(0);

    m_bound = true;
  }

  void Mesh::uploadToGPU() {
    if (!GL.glIsVertexArray(m_vao)) {
      if (m_isDirty) {
        setupBuffers();
      }
      m_isDirty = false;
    }
  }

  void Mesh::renderAABB(const glm::vec3 &min, const glm::vec3 &max) const {
    static GLuint aabbVAO = 0, aabbVBO = 0;
    if (aabbVAO == 0) {
      GL.glGenVertexArrays(1, &aabbVAO);
      GL.glGenBuffers(1, &aabbVBO);
    }

    // 8 corners of the box
    glm::vec3 vertices[8] = {
      {min.x,min.y,min.z},{min.x,min.y,max.z},{min.x,max.y,min.z},{min.x,max.y,max.z},
      {max.x,min.y,min.z},{max.x,min.y,max.z},{max.x,max.y,min.z},{max.x,max.y,max.z}
    };

    // 12 edges as pairs of indices
    static const GLuint indices[24] = {
      0,1, 0,2, 0,4,
      1,3, 1,5,
      2,3, 2,6,
      3,7,
      4,5, 4,6,
      5,7,
      6,7
    };

    GL.glBindVertexArray(aabbVAO);
    GL.glBindBuffer(GL_ARRAY_BUFFER, aabbVBO);
    GL.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    GL.glEnableVertexAttribArray(0);
    GL.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    GL.glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, indices);

    GL.glBindVertexArray(0);
  }

  void Mesh::render() const {
    if (m_indices.empty())
      return;
    if (!m_bound)
      return;

    GL.glBindVertexArray(m_vao);
    if (g::renderModes.has(g::RenderMode::POINTS)) {
      GL.glDrawArrays(GL_POINTS, 0, m_vertices.size());
    } else {
      GL.glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    }
  }

  void Mesh::bind() const {
    GL.glBindVertexArray(m_vao);
    GL.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    GL.glEnableVertexAttribArray(0);
    GL.glEnableVertexAttribArray(1);
    GL.glEnableVertexAttribArray(2);
  }

  void Mesh::renderFace(const int faceIndex) const {
    if (m_indices.empty())
      return;
    if (!m_bound)
      return;

    // each face has 6 indices (2 triangles)
    const GLuint startIndex = faceIndex * 6;
    constexpr GLuint indexCount = 6;

    if (startIndex + indexCount > m_indices.size())
      return;

    GL.glBindVertexArray(m_vao);
    GL.glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void *) (startIndex * sizeof(GLuint)));
  }

  void Mesh::renderFaceForPicking(int faceIndex) const {
    if (m_indices.empty())
      return;

    // Each face has 6 indices (2 triangles)
    const GLuint startIndex = faceIndex * 6;
    constexpr GLuint indexCount = 6;

    if (startIndex + indexCount > m_indices.size())
      return;

    GL.glBindVertexArray(m_vao);
    GL.glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void *) (startIndex * sizeof(GLuint)));
  }

  std::pair<glm::vec3, glm::vec3> Mesh::getAABB() const {
    if (m_vertices.empty())
      return {glm::vec3(0), glm::vec3(0)};
    glm::vec3 minV = m_vertices[0].position;
    glm::vec3 maxV = m_vertices[0].position;
    for (const auto &v: m_vertices) {
      minV = glm::min(minV, v.position);
      maxV = glm::max(maxV, v.position);
    }
    return {minV, maxV};
  }

  Mesh::~Mesh() {
    if (m_vao != 0)
      GL.glDeleteVertexArrays(1, &m_vao);
    if (m_vbo != 0)
      GL.glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0)
      GL.glDeleteBuffers(1, &m_ebo);
    m_bound = false;
  }
}
