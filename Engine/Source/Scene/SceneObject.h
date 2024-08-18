#pragma once

#include <Core/Mesh/Mesh.h>

namespace South
{

class SSceneObject
{
public:
    virtual ~SSceneObject() = default;

    glm::mat4 ToWorld = glm::mat4(1.f);

    std::string DisplayName;
};

class SSceneMeshObject : public SSceneObject
{
public:
    void AddMesh(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices);
    void AddMesh(const SMeshDescription& MeshDescription);

    std::vector<std::unique_ptr<SMesh>> Meshes;
};

} // namespace South