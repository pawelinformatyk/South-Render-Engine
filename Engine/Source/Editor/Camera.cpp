#include "sthpch.h"

#include "Camera.h"
#include <gtc/matrix_transform.hpp>

namespace South
{

    Camera::Camera(glm::vec3 pos, glm::vec3 target, float fov, float aspect, float near, float far)
    {
        SetView(pos, target);
        SetProjection(fov, aspect, near, far);
    }

    Camera::Camera()
    {
    }

    void Camera::ProcessEvent()
    {
    }

    void Camera::SetView(glm::vec3 pos, glm::vec3 target)
    {
        view = glm::lookAt(pos, target, glm::vec3(0.f, 0.f, 1.f));
    }

    void Camera::SetProjection(float fov, float aspect, float near, float far)
    {
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

} // namespace South