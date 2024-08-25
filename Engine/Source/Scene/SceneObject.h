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
    void AddMesh(const SMeshDescription& MeshDescription);

    std::vector<std::unique_ptr<SMesh>> Meshes;
};

// Examples to be done:

class SSceneTextObject
{
};

class SScenePointLightObject
{
};


} // namespace South