#pragma once

namespace South
{
    class Entity;

    class Scene
    {
      private:
        // Entities on current scene.
        std::vector<Entity*> Entities;
    };

} // namespace South