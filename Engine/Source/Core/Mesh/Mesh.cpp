#include "Mesh.h"

namespace South
{

const VkVertexInputBindingDescription& Vertex::GetBindingDescription()
{
    return m_BindingDesc;
}

const std::array<VkVertexInputAttributeDescription, 3>& Vertex::GetAttributesDescriptions()
{
    return m_AttributesDescs;
}

VkVertexInputBindingDescription Vertex::m_BindingDesc {
    .binding   = 0,
    .stride    = sizeof(Vertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
};

std::array<VkVertexInputAttributeDescription, 3> Vertex::m_AttributesDescs = {
    VkVertexInputAttributeDescription {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, Location),
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
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, Color),
    },
};

SMesh::SMesh()
{
    Buffer = std::make_unique<SVertexIndexBuffer>();
}

SPlaneMesh::SPlaneMesh()
{
    const SVectorFlt Color  = SVectorFlt::Rand(0, 1);
    const SVectorFlt Normal = SVectorFlt::UpVector;

    Vertices.reserve(4);
    Vertices.emplace_back(SVectorFlt(-0.5, 0, -0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, 0, -0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, 0, 0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, 0, 0.5), Normal, Color);

    Indices = {0, 1, 2, 2, 3, 0};
}

SCubeMesh::SCubeMesh()
{
    const SVectorFlt Color  = SVectorFlt::Rand(0, 1);
    const SVectorFlt Normal = SVectorFlt::UpVector;

    Vertices.reserve(8);
    // #TODO: Cube
    Vertices.emplace_back(SVectorFlt(-0.5, -0.5, -0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, -0.5, -0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, -0.5, 0.5), Normal, Color);
    Vertices.emplace_back(SVectorFlt(0.5, -0.5, 0.5), Normal, Color);

    Vertices.emplace_back(Vertices[0].Location + Normal, Normal, Color);
    Vertices.emplace_back(Vertices[1].Location + Normal, Normal, Color);
    Vertices.emplace_back(Vertices[2].Location + Normal, Normal, Color);
    Vertices.emplace_back(Vertices[3].Location + Normal, Normal, Color);

    Indices = {0, 1, 2, 2, 3, 0};
}

SSphereMesh::SSphereMesh(int SegmentsCount)
{
    const SVectorFlt Color = SVectorFlt::Rand(0, 1);

    // #TODO: Reserve
    Vertices.reserve(4);

    for(int IdxX = 0; IdxX <= SegmentsCount; ++IdxX)
    {
        const float SegmentX = (float)IdxX / (float)SegmentsCount;

        for(int IdxY = 0; IdxY <= SegmentsCount; ++IdxY)
        {
            const float SegmentY = (float)IdxY / (float)SegmentsCount;

            const SVectorFlt Loc = SVectorFlt(std::cos(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI),
                                              std::cos(SegmentY * M_PI),
                                              std::sin(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI));

            Vertices.emplace_back(Loc, Loc.GetNormalized(), Color);
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

STriangleMesh::STriangleMesh()
{
}

SCircleMesh::SCircleMesh()
{
}

} // namespace South