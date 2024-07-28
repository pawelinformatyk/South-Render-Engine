#include "Mesh.h"

namespace South
{

const VkVertexInputBindingDescription& Vertex::GetBindingDescription()
{
    return m_BindingDesc;
}

const std::array<VkVertexInputAttributeDescription, 4>& Vertex::GetAttributesDescriptions()
{
    return m_AttributesDescs;
}

VkVertexInputBindingDescription Vertex::m_BindingDesc {
    .binding   = 0,
    .stride    = sizeof(Vertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
};

std::array<VkVertexInputAttributeDescription, 4> Vertex::m_AttributesDescs = {
    VkVertexInputAttributeDescription {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, Pos),
    },
    VkVertexInputAttributeDescription {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, Normal),
    },
    VkVertexInputAttributeDescription {
        .location = 2,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = offsetof(Vertex, TexCoords),
    },
    VkVertexInputAttributeDescription {
        .location = 3,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = offsetof(Vertex, Color),
    },
};

Mesh::Mesh()
{
    Buffer = std::make_unique<VertexIndexBuffer>();
}

PlaneMesh::PlaneMesh()
{
    const VectorFlt Color = VectorFlt::Rand(0, 1);
    const VectorFlt Normal(0, 0, 1);

    Vertices.reserve(4);
    Vertices.emplace_back(VectorFlt(-1, -1, 0), Normal, glm::vec2(1, 0), Color);
    Vertices.emplace_back(VectorFlt(1, -1, 0), Normal, glm::vec2(0, 0), Color);
    Vertices.emplace_back(VectorFlt(1, 1, 0), Normal, glm::vec2(0, 1), Color);
    Vertices.emplace_back(VectorFlt(1, 1, 0), Normal, glm::vec2(1, 1), Color);
}

CubeMesh::CubeMesh()
{
    const VectorFlt Color = VectorFlt::Rand(0, 1);
    const VectorFlt Normal(0, 0, 1);

    Vertices.reserve(8);
    Vertices.emplace_back(VectorFlt(-1, -1, 0), Normal, glm::vec2(1, 0), Color);
    Vertices.emplace_back(VectorFlt(1, -1, 0), Normal, glm::vec2(0, 0), Color);
    Vertices.emplace_back(VectorFlt(1, 1, 0), Normal, glm::vec2(0, 1), Color);
    Vertices.emplace_back(VectorFlt(1, 1, 0), Normal, glm::vec2(1, 1), Color);
}

SphereMesh::SphereMesh(int SegmentsCount)
{
    const VectorFlt Color = VectorFlt::Rand(0, 1);

    // #TODO: Reserve
    Vertices.reserve(4);

    for(int X = 0; X <= SegmentsCount; ++X)
    {
        const float xSegment = (float)X / (float)SegmentsCount;

        for(int Y = 0; Y <= SegmentsCount; ++Y)
        {
            const float ySegment = (float)Y / (float)SegmentsCount;

            const VectorFlt Loc = VectorFlt(std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI),
                                            std::cos(ySegment * M_PI),
                                            std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI));

            Vertices.emplace_back(Loc, Loc, glm::vec2(xSegment, ySegment), Color);
        }
    }

    // #TODO: uint16_t

    bool bOddRow = false;
    for(unsigned int Y = 0; Y < SegmentsCount; ++Y)
    {
        if(!bOddRow) // even rows: y == 0, y == 2; and so on
        {
            for(unsigned int X = 0; X <= SegmentsCount; ++X)
            {
                Indices.push_back(Y * (SegmentsCount + 1) + X);
                Indices.push_back((Y + 1) * (SegmentsCount + 1) + X);
            }
        }
        else
        {
            for(int X = SegmentsCount; X >= 0; --X)
            {
                Indices.push_back((Y + 1) * (SegmentsCount + 1) + X);
                Indices.push_back(Y * (SegmentsCount + 1) + X);
            }
        }
        bOddRow = !bOddRow;
    }
}

TriangleMesh::TriangleMesh()
{
}

CircleMesh::CircleMesh()
{
}

} // namespace South