#pragma once

#include <Core/Mesh/Mesh.h>

namespace South
{

class SceneObject
{
public:
    glm::mat4 ToWorld;
};

class SceneMeshObject : public SceneObject
{
public:
    std::vector<std::unique_ptr<Mesh>> Meshes;
};

} // namespace South