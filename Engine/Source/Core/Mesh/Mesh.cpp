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

    for(int IdxX = 0; IdxX <= SegmentsCount; ++IdxX)
    {
        const float SegmentX = (float)IdxX / (float)SegmentsCount;

        for(int IdxY = 0; IdxY <= SegmentsCount; ++IdxY)
        {
            const float SegmentY = (float)IdxY / (float)SegmentsCount;

            const VectorFlt Loc = VectorFlt(std::cos(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI),
                                            std::cos(SegmentY * M_PI),
                                            std::sin(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI));

            Vertices.emplace_back(Loc, Loc, glm::vec2(SegmentX, SegmentY), Color);
        }
    }

    // #TODO: uint16_t

    bool bOddRow = false;
    for(unsigned int IdxY = 0; IdxY < SegmentsCount; ++IdxY)
    {
        if(!bOddRow) // even rows: y == 0, y == 2; and so on
        {
            for(unsigned int IdxX = 0; IdxX <= SegmentsCount; ++IdxX)
            {
                Indices.push_back(IdxY * (SegmentsCount + 1) + IdxX);
                Indices.push_back((IdxY + 1) * (SegmentsCount + 1) + IdxX);
            }
        }
        else
        {
            for(int IdxX = SegmentsCount; IdxX >= 0; --IdxX)
            {
                Indices.push_back((IdxY + 1) * (SegmentsCount + 1) + IdxX);
                Indices.push_back(IdxY * (SegmentsCount + 1) + IdxX);
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