#pragma once

#include <Core/Mesh/Mesh.h>

namespace South
{

class SSceneObject
{
public:
    glm::mat4 ToWorld;
};

class SSceneMeshObject : public SSceneObject
{
public:
    std::vector<std::unique_ptr<SMesh>> Meshes;
};

} // namespace South