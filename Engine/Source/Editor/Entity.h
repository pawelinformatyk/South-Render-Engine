#pragma once

#include "glm.hpp"

namespace South
{
    class StaticMesh;

    // Visible/rendered entity?
    // #TODO : Should be passed to rendering pipeline?class Entity
    class Entity
    {
      private:
        glm::mat4 Transform;

        StaticMesh* mesh = nullptr;
    };

} // namespace South