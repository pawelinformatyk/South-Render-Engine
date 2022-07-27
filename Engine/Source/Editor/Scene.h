#pragma once

namespace South
{
    class Entity;

    class Scene
    {
    private:
        // Entities on current scene.
        std::vector<Entity*> m_Entities;
    };

} // namespace South