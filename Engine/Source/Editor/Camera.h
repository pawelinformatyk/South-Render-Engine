#pragma once

#include "glm.hpp"

namespace South
{

class Camera
{
public:
    Camera() = default;
    Camera(glm::vec3 pos, glm::vec3 target, float fov, float aspect, float near, float inFar);

    void SetView(glm::vec3 pos, glm::vec3 target);
    void SetProjection(float fov, float aspect, float near, float inFar);

    const glm::mat4& GetView() const;
    const glm::mat4& GetProjection() const;
    glm::mat4        GetViewProjection() const;

    glm::quat GetOrientation() const;

    glm::vec3 GetForwardVector() const;
    glm::vec3 GetRightVector() const;
    glm::vec3 GetUpVector() const;

    void MoveForward(float Delta);
    void MoveRight(float Delta);
    void MoveUp(float Delta);

private:
    void UpdateView();

    glm::mat4 m_View = glm::mat4(1.f);

    glm::vec3 m_Position  = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_Direction = glm::vec3(1.f, 0.f, 0.f);

    glm::mat4 m_Projection = glm::mat4(1.f);

    float m_Fov    = 0.f;
    float m_Aspect = 0.f;
    float m_Near   = 0.f;
    float m_Far    = 0.f;
};

} // namespace South
