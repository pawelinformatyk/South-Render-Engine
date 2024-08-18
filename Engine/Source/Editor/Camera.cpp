#include "Camera.h"

namespace South
{

SCamera::SCamera()
{
    UpdateProjection();
    UpdateView();
}

glm::mat4 SCamera::GetViewProjection() const
{
    return View * Projection;
}

SVectorFlt SCamera::GetForwardVector() const
{
    return Convert(glm::normalize(glm::vec3(cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
                                            sin(glm::radians(Pitch)),
                                            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)))));
}

SVectorFlt SCamera::GetRightVector() const
{
    return Convert(glm::cross(Convert(GetForwardVector()), Convert(GetUpVector())));
}

SVectorFlt SCamera::GetUpVector() const
{
    return SVectorFlt::UpVector;
}

std::string SCamera::ToString() const
{
    return "Camera loc " + std::to_string(Location.X) + " / " + std::to_string(Location.Y) + " / " + std::to_string(Location.Z) +
           " / pitch " + std::to_string(Pitch) + " / Yaw " + std::to_string(Yaw);
}

void SCamera::MoveForward(const float Delta)
{
    const SVectorFlt DeltaDir = MoveSpeed * Delta * GetForwardVector();

    Location += DeltaDir;

    UpdateView();
}

void SCamera::MoveRight(const float Delta)
{
    const SVectorFlt DeltaDir = MoveSpeed * Delta * GetRightVector();

    Location += DeltaDir;

    UpdateView();
}

void SCamera::MoveUp(const float Delta)
{
    const SVectorFlt DeltaDir = MoveSpeed * Delta * GetUpVector();

    Location += DeltaDir;

    UpdateView();
}

void SCamera::LookRight(const float Delta)
{
    Yaw += RotationSpeed * Delta;

    // #TODO: Prevent gimbal lock

    UpdateView();
}

void SCamera::LookUp(const float Delta)
{
    Pitch += RotationSpeed * Delta;


    UpdateView();
}

void SCamera::UpdateProjection()
{
    Projection = glm::perspective(Fov, Aspect, Near, Far);

    // Flip so we don't get upside down image (glm is for OpenGL, where the Y coordinate of the clip coordinates is
    // inverted).
    Projection[1][1] *= -1;
}

void SCamera::UpdateView()
{
    View = glm::lookAt(Convert(Location), Convert(Location + GetForwardVector()), Convert(GetUpVector()));
}

} // namespace South