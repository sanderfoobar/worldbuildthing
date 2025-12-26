#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
    
    // Movement
    void moveForward(float delta, float speed);
    void moveBackward(float delta, float speed);
    void moveLeft(float delta, float speed);
    void moveRight(float delta, float speed);
    void moveUp(float delta, float speed);
    void moveDown(float delta, float speed);
    
    // Rotation via mouse
    void rotate(float deltaYaw, float deltaPitch);
    
    // Matrix generation
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    
    // Getters
    glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getDirection() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;
    
private:
    glm::vec3 m_position;
    float m_yaw;      // Rotation around Y axis (left/right)
    float m_pitch;    // Rotation around X axis (up/down)
    
    static constexpr float m_speed = 8.0f;  // Movement speed (units per second)
    static constexpr float m_mouseSensitivity = 0.1f;  // Mouse rotation sensitivity
    static constexpr float m_fov = 45.0f;
    static constexpr float m_near = 0.1f;
    static constexpr float m_far = 15000.0f;
};

}  // namespace gl
