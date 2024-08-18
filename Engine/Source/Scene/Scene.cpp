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
        std::vector<Vertex>   Vertices;
        std::vector<uint32_t> Indices;

        SVectorFlt Color;
        if(Shape.name == "ShipBrown")
        {
            Color = SVectorFlt(0.64, 0.16, 0.16);
        }
        else if(Shape.name == "ShipRed")
        {
            Color = SVectorFlt(1, 0, 0);
        }
        else if(Shape.name == "Ground")
        {
            Color = SVectorFlt(0, 0, 0.4);
        }
        else if(Shape.name == "Water")
        {
            Color = SVectorFlt(0, 0, 1);
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

            Vertices.emplace_back(Location, Normal, Color);
            Indices.emplace_back(static_cast<uint32_t>(Indices.size()));
        }

        SceneMeshObject->AddMesh(std::move(Vertices), std::move(Indices));
    }

    SceneObjects.emplace_back(std::move(SceneMeshObject));
}

} // namespace South