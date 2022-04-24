#pragma once

#include "glm.hpp"

namespace South
{

    class Camera
    {
      public:
        Camera();
        Camera(glm::vec3 pos, glm::vec3 target, float fov, float aspect, float near, float inFar);

        void ProcessEvent();

        void SetView(glm::vec3 pos, glm::vec3 target);
        void SetProjection(float fov, float aspect, float near, float inFar);

        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetProjectionMatrix() const;
        glm::mat4 GetViewProjectionMatrix() const;

        void SetPosition(glm::vec3 newPos);
        void AddPosition(glm::vec3 deltaPos);

        void SetRotation(glm::quat newRotation);
        void AddRotation(glm::quat deltaRotation);

        void SetRotation(glm::vec3 newRotation);
        void AddRotation(glm::vec3 deltaRotation);

      private:
        glm::mat4 view;
        glm::mat4 projection;

        // xyz -  pitch, yaw, roll
        // Roll  - around forward.
        // yaw   - around up.
        // pitch - around right.

        glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
        glm::vec3 right   = glm::vec3(1.f, 0.f, 0.f);
        glm::vec3 up      = glm::vec3(0.f, 1.f, 0.f);

        float Fov    = 0.f;
        float Aspect = 0.f;
        float Near   = 0.f;
        float Far    = 0.f;
    };

} // namespace South
