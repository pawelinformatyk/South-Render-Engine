#include "sthpch.h"

#include "Camera.h"
#include <gtc/matrix_transform.hpp>

namespace South
{

    Camera::Camera(glm::vec3 inPos, glm::vec3 inTarget, float inFov, float inAspect, float inNear, float inFar)
        : fov(inFov)
        , aspect(inAspect)
        , near(inNear)
        , far(inFar)
    {
        SetView(inPos, inTarget);
        SetProjection(fov, aspect, near, far);
    }

    Camera::Camera()
    {
    }

    void Camera::ProcessEvent()
    {
    }

    void Camera::SetView(glm::vec3 inPos, glm::vec3 inTarget)
    {
        view = glm::lookAt(inPos, inTarget, glm::vec3(0.f, 0.f, 1.f));
    }

    void Camera::SetProjection(float inFov, float inAspect, float inNear, float inFar)
    {
        fov    = inFov;
        aspect = inAspect;
        near   = inNear;
        far    = inFar;

        projection = glm::perspective(fov, aspect, near, far);
        projection[1][1] *= -1;
    }

    const glm::mat4& Camera::GetViewMatrix() const
    {
        return view;
    }

    const glm::mat4& Camera::GetProjectionMatrix() const
    {
        return projection;
    }

    glm::mat4 Camera::GetViewProjectionMatrix() const
    {
        return view * projection;
    }

} // namespace South