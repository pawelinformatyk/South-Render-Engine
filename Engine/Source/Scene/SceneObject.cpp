#include "SceneObject.h"

namespace South
{

void SSceneMeshObject::AddMesh(std::vector<Vertex> Vertices, std::vector<uint32_t> Indices)
{
    auto Mesh = std::make_unique<SMesh>();

    Mesh->Vertices = std::move(Vertices);
    Mesh->Indices  = std::move(Indices);

    Meshes.push_back(std::move(Mesh));
}

} // namespace South