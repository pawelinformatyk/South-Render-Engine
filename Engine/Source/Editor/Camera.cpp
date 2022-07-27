#include "sthpch.h"

#include "Camera.h"
#include <gtc/matrix_transform.hpp>

namespace South
{

    Camera::Camera(glm::vec3 inPos, glm::vec3 inTarget, float inFov, float inAspect, float inNear, float inFar)
        : m_Fov(inFov)
        , m_Aspect(inAspect)
        , m_Near(inNear)
        , m_Far(inFar)
    {
        SetView(inPos, inTarget);
        SetProjection(m_Fov, m_Aspect, m_Near, m_Far);
    }

    Camera::Camera() {}

    void Camera::ProcessEvent() {}

    void Camera::SetView(glm::vec3 inPos, glm::vec3 inTarget)
    {
        m_View = glm::lookAt(inPos, inTarget, glm::vec3(0.f, 0.f, 1.f));
    }

    void Camera::SetProjection(float inFov, float inAspect, float inNear, float inFar)
    {
        m_Fov    = inFov;
        m_Aspect = inAspect;
        m_Near   = inNear;
        m_Far    = inFar;

        m_Projection = glm::perspective(m_Fov, m_Aspect, m_Near, m_Far);
        m_Projection[1][1] *= -1;
    }

    const glm::mat4& Camera::GetViewMatrix() const { return m_View; }

    const glm::mat4& Camera::GetProjectionMatrix() const { return m_Projection; }

    glm::mat4 Camera::GetViewProjectionMatrix() const { return m_View * m_Projection; }

} // namespace South