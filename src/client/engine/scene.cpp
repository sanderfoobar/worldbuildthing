// /home/dsc/CLionProjects/godot/texture_engine/data/cube_test.vmf
// /home/dsc/CLionProjects/godot/texture_engine/data/measurewall01.png

#include <random>

#include "gameobject.h"
#include "scene.h"
#include "mesh.h"

#include "../lib/vmfpp/vmfpp.h"
#include "lib/globals.h"

namespace gl {

static QMap<const std::filesystem::path, QSharedPointer<Scene>> sceneCache = {};

Scene::Scene(QObject* parent) : QObject(parent) {
  setObjectName("Untitled Scene");
  // m_gizmo = new TransformGizmo(this);
  // m_camera = new Camera(this);
  // m_gridlineNameCounter = 1;
  m_ambientLightNameCounter = 1;
  m_directionalLightNameCounter = 1;
  m_pointLightNameCounter = 1;
  m_spotLightNameCounter = 1;

  auto direc = new DirectionalLight();
  m_directionalLights.push_back(direc);
}

QSharedPointer<Scene> Scene::VMF(const std::filesystem::path &path) {
  if (sceneCache.contains(path))
    return sceneCache[path];

  auto scene = QSharedPointer<Scene>(new Scene(nullptr));

  auto vmf = vmfpp::VMF::openFile(path.string());
  if (!vmf.has_value()) {
    qWarning() << "VMF open failed; faulty VMF?";
    return scene;
  }

  int mesh_idx = 1;
  auto *node_ptr = reinterpret_cast<vmfpp::Node *>(&*vmf);

  for (const auto &[fst, snd] : node_ptr->getChildren()) {
    if (fst != "world" || snd.empty())
      continue;

    auto solids = snd[0].getChild("solid");
    for (const auto &solid : solids) {
      Solid solid_struct{};
      solid_struct.id = std::stoi(solid.getValue("id")[0]);
      auto sides = solid.getChild("side");

      for (const auto &side : sides) {
        Side side_struct{};
        side_struct.rotation = std::stoi(side.getValue("rotation")[0]);
        side_struct.id = std::stoi(side.getValue("id")[0]);
        side_struct.material = side.getValue("material")[0];

        // remove / from material
        size_t pos = side_struct.material.find("/");
        if (pos != std::string::npos) {
          side_struct.material = side_struct.material.substr(pos + 1);
        }

        // register if mat name was all uppercased, and lowercase it
        side_struct.material_name_was_uppercased =
          std::ranges::all_of(side_struct.material, [](char c) {
            return !std::isalpha(c) || std::isupper(c);
          });
        if (side_struct.material_name_was_uppercased)
          std::ranges::transform(side_struct.material, side_struct.material.begin(), [](const unsigned char c){ return std::tolower(c); });

        // parse uaxis/vaxis
        auto vaxis = side.getValue("uaxis")[0];
        auto uaxis = side.getValue("vaxis")[0];
        float a, b, c, d, e;
        if (sscanf(vaxis.c_str(), "[%f %f %f %f] %f", &a, &b, &c, &d, &e) != 5) {
          qWarning() << "vaxis parse error:" << vaxis.c_str();
          continue;
        }
        side_struct.vaxis = {a, b, c, d, e};
        if (sscanf(uaxis.c_str(), "[%f %f %f %f] %f", &a, &b, &c, &d, &e) != 5) {
          qWarning() << "uaxis parse error:" << uaxis.c_str();
          continue;
        }
        side_struct.uaxis = {a, b, c, d, e};

        // parse vertices_plus
        auto side_childs = side.getChildren();
        for (const auto &side_child : side_childs) {
          if (side_child.first != "vertices_plus")
            continue;

          auto vert = side_child.second[0].getValue("v");
          if (vert.size() < 4)
            continue;

          for (size_t i = 0; i < 4; ++i) {
            float x, y, z;
            const char *cstr = vert[i].c_str();
            char *end;
            x = std::strtof(cstr, &end); cstr = end;
            y = std::strtof(cstr, &end); cstr = end;
            z = std::strtof(cstr, &end);
            side_struct.vertices_plus[i] = QVector3D(x / 10, y / 10, z / 10);
          }
        }

        solid_struct.sides.push_back(side_struct);
      }

      if (solid_struct.sides.size() == 6) {
        std::array<Side, 6> cubeSides;
        std::copy_n(solid_struct.sides.begin(), 6, cubeSides.begin());

        auto mesh = Mesh::createFromCubeSides(cubeSides);
        auto cube = std::make_shared<GameObject>(mesh);

        for (int face = 0; face < 6; ++face) {
          int ieowg = 1;
          auto &cube_side = cubeSides.at(face);
          auto &mat_str = cube_side.material;

          // associated mat
          QSharedPointer<Material> mat;
          if (cube_side.material_name_was_uppercased) {
            auto has_tex = gs::TEXTURES_LOWER.contains(QString::fromStdString(cube_side.material));
            if (!has_tex) {
              mat = g::glTextureManager->get_by_name("devtexture01");
            }
          } else {
            auto has_tex = gs::TEXTURES.contains(QString::fromStdString(cube_side.material));
            if (!has_tex) {
              mat = g::glTextureManager->get_by_name("devtexture01");
            }
          }

          if (mat.isNull())
            continue;

          uint32_t pickingID = mesh_idx * 6 + face;
          mat->setPickingID(pickingID);
          mesh->add_face_material(face, mat);

          glm::vec2 uvScale(cubeSides[face].uaxis[4], cubeSides[face].vaxis[4]);
          mesh->setFaceUVScale(face, uvScale);

          // uv offset // @TODO: test if this works
          // auto uvOffset = glm::vec2(cubeSides[face].uaxis[4], cubeSides[face].vaxis[4]);

          mesh->setFaceUVOffset(face, {0,0});

          mesh_idx++;
        }

        scene->gameObjects.push_back(cube);
      } else {
        qWarning() << "Solid id" << solid_struct.id << "does not have exactly 6 sides; skipping";
      }
    }
  }

  for (auto &obj: scene->gameObjects)
    obj->computeWorldAABB();

  sceneCache[path] = scene;
  return scene;
}

QSharedPointer<Scene> Scene::setupSceneFromVMF(std::filesystem::path path) {
  return VMF(path);
}

QSharedPointer<Scene> Scene::setupSceneWithABunchOfBoxes() {
  auto scene = QSharedPointer<Scene>(new Scene());
  // create 6 cubes in a circle formation
  float radius = 10.0f;
  float range = 200.0f;
  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> dist(-range, range);

  for (int i = 0; i < 10000; ++i) {
    auto cubeMesh = Mesh::create(1.0f);
    auto cube = std::make_shared<GameObject>(cubeMesh);

    float x = dist(rng);
    float y = dist(rng);
    float z = dist(rng);
    cube->setPosition({x, y, z});
    cube->setPosition(glm::vec3(x, 0.0f, z));
    cube->setRotation(glm::vec3(i * 10.0f, 0, 0.0f));

    // assign different colors to each cube's faces
    glm::vec3 colors[] = {
      glm::vec3(1.0f, 0.0f, 0.0f), // red
      glm::vec3(0.0f, 1.0f, 0.0f), // green
      glm::vec3(0.0f, 0.0f, 1.0f), // blue
      glm::vec3(1.0f, 1.0f, 0.0f), // yellow
      glm::vec3(1.0f, 0.0f, 1.0f), // magenta
      glm::vec3(0.0f, 1.0f, 1.0f), // cyan
    };

    for (int face = 0; face < 6; ++face) {
      // // prefer textures from the texture DB; fall back to colored material
      // if (TEX_DB->count() > 0) {
      //   // choose an entry index by cube and face so each side can be different
      //   size_t entryIdx = (i * 6 + face) % TEX_DB->count();
      //   Material mat = TEX_DB->createMaterialFromIndex(entryIdx);
      //   // apply a default repeating scale (demonstration); users can adjust mesh API
      //   // use a consistent default UV scale for all faces so scaling
      //   // behaves the same regardless of face index.
      //   glm::vec2 defaultScale = glm::vec2(1.0f, 1.0f);
      //   cubeMesh->setFaceUVScale(face, defaultScale);
      //   cubeMesh->setFaceUVOffset(face, glm::vec2(0.0f, 0.0f));
      //   // assign unique picking ID: cube_index * 6 + face_index
      //   uint32_t pickingID = i * 6 + face;
      //   mat.setPickingID(pickingID);
      //   cubeMesh->addFaceMaterial(face, mat);
      // } else {
      //   Material colorMat(colors[face]);
      //   uint32_t pickingID = i * 6 + face;
      //   colorMat.setPickingID(pickingID);
      //   cubeMesh->addFaceMaterial(face, colorMat);
      // }
    }

    scene->gameObjects.push_back(cube);
  }

  // cache world-space AABB for all objects (static scene)
  for (auto &obj: scene->gameObjects) {
    obj->computeWorldAABB();
  }

  return scene;
}
}