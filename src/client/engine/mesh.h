#pragma once

#include <array>
#include <memory>
#include <vector>

#include <GL/gl.h>
#include <QVector3D>
#include <glm/glm.hpp>

#include "client/gl2/material.h"

namespace gl {
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
  };

  struct Side {
    int id = -1;
    int rotation = 0;
    std::array<float, 5> uaxis = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::array<float, 5> vaxis = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    std::string material;
    bool material_name_was_uppercased = false;
    std::array<QVector3D, 4> vertices_plus;
  };

  struct Solid {
    int id = -1;
    std::vector<Side> sides;
  };

  class Mesh {
  public:
    // Returns axis-aligned bounding box (min, max)
    std::pair<glm::vec3, glm::vec3> getAABB() const;

    Mesh();
    ~Mesh();

    // static std::shared_ptr<Mesh> create(float size);
    void renderAABB(const glm::vec3 &min, const glm::vec3 &max) const;

  static std::shared_ptr<Mesh> create(float size) {
    auto mesh = std::make_shared<Mesh>();
    float half = size / 2.0f;

    // Define 24 vertices (4 per face with separate normals and UVs)
    std::vector<Vertex> vertices = {
        // Front face (Z+)
        {{-half, -half, half}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{half, -half, half}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{half, half, half}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-half, half, half}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        // Back face (Z-)
        {{half, -half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-half, -half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-half, half, -half}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{half, half, -half}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},

        // Left face (X-)
        {{-half, -half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{-half, -half, half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-half, half, half}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-half, half, -half}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Right face (X+)
        {{half, -half, half}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{half, -half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{half, half, -half}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{half, half, half}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        // Top face (Y+)
        {{-half, half, half}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{half, half, half}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{half, half, -half}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-half, half, -half}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        // Bottom face (Y-)
        {{-half, -half, -half}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
        {{half, -half, -half}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
        {{half, -half, half}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-half, -half, half}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
    };

    // Indices (2 triangles per face, 6 indices per face)
    std::vector<GLuint> indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Back
        4, 5, 6, 6, 7, 4,
        // Left
        8, 9, 10, 10, 11, 8,
        // Right
        12, 13, 14, 14, 15, 12,
        // Top
        16, 17, 18, 18, 19, 16,
        // Bottom
        20, 21, 22, 22, 23, 20,
    };

    mesh->addVertices(vertices);
    mesh->addIndices(indices);
    return mesh;
  }

    static std::shared_ptr<Mesh> createFromCubeSides(const std::array<Side, 6> &sides);

    // add geometry
    void addVertices(const std::vector<Vertex> &vertices);
    void addIndices(const std::vector<GLuint> &indices);
    void add_face_material(int faceIndex, const QSharedPointer<Material> &material);

    std::vector<Vertex> vertices() const { return m_vertices; }
    std::vector<GLuint> indices() const { return m_indices; }

    // rendering
    void render() const;
    void renderFace(int faceIndex) const; // render specific face for material testing
    void renderFaceForPicking(int faceIndex) const; // render face with picking ID

    void clearGeometry();  // clear and rebuild
    void uploadToGPU();

    size_t getIndexCount() const { return m_indices.size(); }
    GLuint getVAO() const { return m_vao; }

    // Face management (per-face arrays sized to number of faces)
    QSharedPointer<Material> &getFaceMaterial(const int faceIndex) { return m_faceMaterials[faceIndex % m_faceMaterials.size()]; }
    const QSharedPointer<Material> &getFaceMaterial(const int faceIndex) const { return m_faceMaterials[faceIndex % m_faceMaterials.size()]; }

    // Per-face UV transform accessors
    void setFaceUVScale(int faceIndex, const glm::vec2 &scale);
    void setFaceUVOffset(int faceIndex, const glm::vec2 &offset);
    glm::vec2 getFaceUVScale(int faceIndex) const;
    glm::vec2 getFaceUVOffset(int faceIndex) const;

    void bind() const;
    bool is_bound() const { return m_bound; }

  private:
    GLuint m_vao, m_vbo, m_ebo;

    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    std::vector<QSharedPointer<Material>> m_faceMaterials;
    std::vector<glm::vec2> m_faceUVScales; // Per-face UV scale
    std::vector<glm::vec2> m_faceUVOffsets; // Per-face UV offset
    bool m_isDirty = false;
    bool m_bound = false;

    void setupBuffers();
  };

} // namespace gl
