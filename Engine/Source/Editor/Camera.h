#pragma once

#include "glm.hpp"

namespace South
{

    class Camera
    {
      public:
        Camera();
        Camera(glm::vec3 pos, glm::vec3 target, float fov, float aspect, float near, float far);

        void ProcessEvent();

        void SetView(glm::vec3 pos, glm::vec3 target);
        void SetProjection(float fov, float aspect, float near, float far);

        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionMatrix() const;

      private:
        glm::mat4 view;
        glm::mat4 projection;
    };
} // namespace South
