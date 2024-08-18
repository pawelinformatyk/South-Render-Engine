#pragma once

#include <Core/Mesh/Mesh.h>

namespace South
{

class SSceneObject
{
public:
    virtual ~SSceneObject() = default;

    glm::mat4 ToWorld = glm::mat4(1);

    std::string DisplayName;
};

class SSceneMeshObject : public SSceneObject
{
public:
    void AddMesh(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices);

    std::vector<std::unique_ptr<SMesh>> Meshes;
};

} // namespace South