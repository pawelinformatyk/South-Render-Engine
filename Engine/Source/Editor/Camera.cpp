#include "Camera.h"

namespace South
{

Camera::Camera()
{
    UpdateProjection();
    UpdateView();
}

glm::mat4 Camera::GetViewProjection() const
{
    return View * Projection;
}

VectorFlt Camera::GetForwardVector() const
{
    return Convert(glm::normalize(glm::vec3(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
                                            sin(glm::radians(Pitch)),
                                            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)))));
}

VectorFlt Camera::GetRightVector() const
{
    return Convert(glm::cross(Convert(GetForwardVector()), Convert(GetUpVector())));
}

VectorFlt Camera::GetUpVector() const
{
    return VectorFlt::UpVector;
}

void Camera::MoveForward(const float Delta)
{
    const VectorFlt DeltaDir = MoveSpeed * Delta * GetForwardVector();

    Location += DeltaDir;

    UpdateView();
}

void Camera::MoveRight(const float Delta)
{
    const VectorFlt DeltaDir = MoveSpeed * Delta * GetRightVector();

    Location += DeltaDir;

    UpdateView();
}

void Camera::MoveUp(const float Delta)
{
    const VectorFlt DeltaDir = MoveSpeed * Delta * GetUpVector();

    Location += DeltaDir;

    UpdateView();
}

void Camera::LookRight(const float Delta)
{
    Yaw += RotationSpeed * Delta;

    // #TODO: Prevent gimbal lock

    UpdateView();
}

void Camera::LookUp(const float Delta)
{
    Pitch += RotationSpeed * Delta;


    UpdateView();
}

void Camera::UpdateProjection()
{
    Projection = glm::perspective(Fov, Aspect, Near, Far);

    // Flip so we don't get upside down image (glm is for OpenGL, where the Y coordinate of the clip coordinates is
    // inverted).
    Projection[1][1] *= -1;
}

void Camera::UpdateView()
{
    View = glm::lookAt(Convert(Location), Convert(Location + GetForwardVector()), Convert(GetUpVector()));
}

} // namespace South