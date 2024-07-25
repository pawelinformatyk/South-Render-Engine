#include "Camera.h"

namespace South
{

Camera::Camera(const VectorFlt& Pos, const VectorFlt& Target, const float Fov, const float Aspect, const float Near, const float Far) :
    m_Fov(Fov), m_Aspect(Aspect), m_Near(Near), m_Far(Far)
{
    SetView(Pos, Target);
    SetProjection(m_Fov, m_Aspect, m_Near, m_Far);
}

void Camera::SetView(const VectorFlt& Pos, const VectorFlt& Target)
{
    m_View = glm::lookAt(Convert(Pos), Convert(Target), Convert(VectorFlt::UpVector));
}

void Camera::SetProjection(const float Fov, const float Aspect, const float Near, const float Far)
{
    m_Fov    = Fov;
    m_Aspect = Aspect;
    m_Near   = Near;
    m_Far    = Far;

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
    return glm::quat(Convert(VectorFlt(m_Pitch, m_Yaw, 0.f)));
}

VectorFlt Camera::GetForwardVector() const
{
    const auto Vec = rotate(GetOrientation(), Convert(VectorFlt::ForwardVector));

    return {Vec.x, Vec.y, Vec.z};
}

VectorFlt Camera::GetRightVector() const
{
    const auto Vec = rotate(GetOrientation(), Convert(VectorFlt::RightVector));

    return {Vec.x, Vec.y, Vec.z};
}

VectorFlt Camera::GetUpVector() const
{
    const auto Vec = rotate(GetOrientation(), Convert(VectorFlt::UpVector));

    return {Vec.x, Vec.y, Vec.z};
}

void Camera::MoveForward(const float Delta)
{
    const VectorFlt Forward = m_MoveSpeed * Delta * GetForwardVector();

    m_Position += Forward;

    UpdateView();
}

void Camera::MoveRight(const float Delta)
{
    const VectorFlt RightVector = m_MoveSpeed * Delta * GetRightVector();

    m_Position += RightVector;

    UpdateView();
}

void Camera::MoveUp(const float Delta)
{
    const VectorFlt UpVector = m_MoveSpeed * Delta * GetUpVector();

    m_Position += UpVector;

    UpdateView();
}

void Camera::LookRight(const float Delta)
{
    m_Yaw += m_RotationSpeed * Delta;

    UpdateDirection();
    UpdateView();
}

void Camera::LookUp(const float Delta)
{
    m_Pitch += m_RotationSpeed * Delta;

    UpdateDirection();
    UpdateView();
}

void Camera::UpdateView()
{
    m_View = glm::lookAt(Convert(m_Position), Convert(m_Direction), Convert(VectorFlt::UpVector));
}

void Camera::UpdateDirection()
{
    m_Direction.X = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Direction.Y = sin(glm::radians(m_Pitch));
    m_Direction.Z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
}

} // namespace South