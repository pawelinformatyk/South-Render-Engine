#include "Scene.h"

#include "Core/Mesh/tiny_obj_loader.h"
#include "Core/Renderer/RendererContext.h"

namespace South
{

void SScene::LoadExample()
{
    tinyobj::attrib_t             Attrib;
    std::vector<tinyobj::shape_t> Shapes;

    const std::string FileName = "Engine/Resources/Models/JunkShip.obj";

    if(!tinyobj::LoadObj(&Attrib, &Shapes, nullptr, nullptr, nullptr, FileName.c_str()))
    {
        return;
    }

    auto SceneMeshObject         = std::make_unique<SSceneMeshObject>();
    SceneMeshObject->DisplayName = "JunkShip";

    for(const auto& Shape : Shapes)
    {
        SMeshDescription MeshDesc;

        // #TODO: Colors are now fucked
        SLinearColor Color;
        if(Shape.name == "ShipBrown")
        {
            Color = SLinearColor(0.64, 0.16, 0.16);
        }
        else if(Shape.name == "ShipRed")
        {
            Color = SLinearColor(1, 0, 0);
        }
        else if(Shape.name == "Ground")
        {
            Color = SLinearColor(0, 0, 0.4);
        }
        else if(Shape.name == "Water")
        {
            Color = SLinearColor(0, 0, 1);
        }

        for(const auto& Index : Shape.mesh.indices)
        {
            const SVectorFlt Location {Attrib.vertices[3 * Index.vertex_index + 1],
                                       Attrib.vertices[3 * Index.vertex_index + 2],
                                       Attrib.vertices[3 * Index.vertex_index + 0]};

            const SVectorFlt Normal = SVectorFlt {Attrib.normals[3 * Index.normal_index + 1],
                                                  Attrib.normals[3 * Index.normal_index + 2],
                                                  Attrib.normals[3 * Index.normal_index + 0]}
                                          .GetNormalized();

            MeshDesc.Vertices.emplace_back(Location, Normal, Color);
            MeshDesc.Indices.emplace_back(static_cast<uint32_t>(MeshDesc.Indices.size()));
        }

        SceneMeshObject->AddMesh(MeshDesc);
    }

    SceneObjects.emplace_back(std::move(SceneMeshObject));

    {
        auto SceneMeshObject2         = std::make_unique<SSceneMeshObject>();
        SceneMeshObject2->DisplayName = "Plane";

        // #TODO: Somehow it is flipped - frontFace idk
        SceneMeshObject2->ToWorld = glm::scale(SceneMeshObject2->ToWorld, glm::vec3(100));
        SceneMeshObject2->ToWorld = glm::rotate(SceneMeshObject2->ToWorld, glm::radians(180.f), Convert(SVectorFlt::RightVector));

        // #TODO: Plane is gray but that's prob because of the lighting (camera loc) not so sure.
        SceneMeshObject2->AddMesh(CreatePlaneMesh(SLinearColor(0, 1, 0)));

        SceneObjects.emplace_back(std::move(SceneMeshObject2));
    }
}

} // namespace South