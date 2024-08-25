#include "SceneObject.h"

namespace South
{

void SSceneMeshObject::AddMesh(const SMeshDescription& MeshDescription)
{
    auto Mesh = std::make_unique<SMesh>();

    Mesh->Vertices = MeshDescription.Vertices;
    Mesh->Indices  = MeshDescription.Indices;

    Meshes.push_back(std::move(Mesh));
}

} // namespace South