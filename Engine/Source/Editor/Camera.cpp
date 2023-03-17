#include "sthpch.h"

#include "Camera.h"

#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>

namespace South
{

Camera::Camera(glm::vec3 inPos, glm::vec3 inTarget, float inFov, float inAspect, float inNear, float inFar) :
    m_Fov(inFov), m_Aspect(inAspect), m_Near(inNear), m_Far(inFar)
{
    SetView(inPos, inTarget);
    SetProjection(m_Fov, m_Aspect, m_Near, m_Far);
}

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

const glm::mat4& Camera::GetView() const
{
    return m_View;
}

const glm::mat4& Camera::GetProjection() const
{
    return m_Projection;
}

glm::mat4 Camera::GetViewProjection() const
{
    return m_View * m_Projection;
}

glm::quat Camera::GetOrientation() const
{
    return glm::quat(glm::vec3(1.f, 0.f, 0.f));
}

glm::vec3 Camera::GetForwardVector() const
{
    return glm::rotate(GetOrientation(), glm::vec3(1.f, 0.f, 0.f));
}

glm::vec3 Camera::GetRightVector() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.f, 1.f, 0.f));
}

glm::vec3 Camera::GetUpVector() const
{
    return glm::rotate(GetOrientation(), glm::vec3(0.f, 0.f, 1.0f));
}

void Camera::MoveForward(float Delta)
{
    const glm::vec3 Forward = Delta * GetForwardVector();

    m_Position += Forward;

    UpdateView();
}

void Camera::MoveRight(float Delta)
{
    const glm::vec3 RightVector = Delta * GetRightVector();

    m_Position += RightVector;

    UpdateView();
}

void Camera::MoveUp(float Delta)
{
    const glm::vec3 UpVector = Delta * GetUpVector();

    m_Position += UpVector;

    UpdateView();
}

void Camera::UpdateView()
{
    m_View = glm::lookAt(m_Position, m_Direction, glm::vec3(0.f, 0.f, 1.f));
}

} // namespace South