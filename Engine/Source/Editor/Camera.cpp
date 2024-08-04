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

glm::quat Camera::GetOrientation() const
{
    return glm::quat {glm::normalize(glm::vec3(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
                                               sin(glm::radians(Pitch)),
                                               sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))))};
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
    const auto PrevYaw = Yaw;
    Yaw += RotationSpeed * Delta;

    // UpdateView is expensive.
    if(PrevYaw != Yaw)
    {
        UpdateView();
    }
}

void Camera::LookUp(const float Delta)
{
    const auto PrevPitch = Pitch;
    Pitch += RotationSpeed * Delta;

    // UpdateView is expensive.
    if(PrevPitch != Pitch)
    {
        UpdateView();
    }
}

void Camera::UpdateProjection()
{
    Projection = glm::perspective(Fov, Aspect, Near, Far);
    Projection[1][1] *= -1;
}

void Camera::UpdateView()
{
    const VectorFlt Target = Location + GetForwardVector();

    View = glm::lookAt(Convert(Location), Convert(Target), Convert(VectorFlt::UpVector));
}

} // namespace South