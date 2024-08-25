#include "Mesh.h"

namespace South
{

const VkVertexInputBindingDescription& SVertex::GetBindingDescription()
{
    return m_BindingDesc;
}

const std::array<VkVertexInputAttributeDescription, 3>& SVertex::GetAttributesDescriptions()
{
    return m_AttributesDescs;
}

VkVertexInputBindingDescription SVertex::m_BindingDesc {
    .binding   = 0,
    .stride    = sizeof(SVertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
};

std::array<VkVertexInputAttributeDescription, 3> SVertex::m_AttributesDescs = {
    VkVertexInputAttributeDescription {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(SVertex, Location),
    },
    VkVertexInputAttributeDescription {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(SVertex, Normal),
    },
    VkVertexInputAttributeDescription {
        .location = 2,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32A32_SFLOAT,
        .offset   = offsetof(SVertex, Color),
    },
};

SMeshDescription CreatePlaneMesh(const SLinearColor& Color)
{
    SMeshDescription Out;

    const SVectorFlt Normal = SVectorFlt::UpVector;

    Out.Vertices.reserve(4);
    Out.Vertices.emplace_back(SVectorFlt(-0.5, 0, -0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(0.5, 0, -0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(0.5, 0, 0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(-0.5, 0, 0.5), Normal, Color);

    Out.Indices = {0, 1, 2, 2, 3, 0};

    return Out;
}

SMeshDescription CreateCubeMesh(const SLinearColor& Color)
{
    SMeshDescription Out;

    const SVectorFlt Normal = SVectorFlt::UpVector;

    Out.Vertices.reserve(8);
    // #TODO: Cube
    Out.Vertices.emplace_back(SVectorFlt(-0.5, -0.5, -0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(0.5, -0.5, -0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(0.5, -0.5, 0.5), Normal, Color);
    Out.Vertices.emplace_back(SVectorFlt(-0.5, -0.5, 0.5), Normal, Color);

    Out.Vertices.emplace_back(Out.Vertices[0].Location + Normal, Normal, Color);
    Out.Vertices.emplace_back(Out.Vertices[1].Location + Normal, Normal, Color);
    Out.Vertices.emplace_back(Out.Vertices[2].Location + Normal, Normal, Color);
    Out.Vertices.emplace_back(Out.Vertices[3].Location + Normal, Normal, Color);

    Out.Indices = {0, 1, 2, 2, 3, 0};

    return Out;
}

SMeshDescription CreateSphereMesh(const SLinearColor& Color, int SegmentsCount)
{
    SMeshDescription Out;

    // #TODO: Reserve
    Out.Vertices.reserve(4);

    for(int IdxX = 0; IdxX <= SegmentsCount; ++IdxX)
    {
        const float SegmentX = (float)IdxX / (float)SegmentsCount;

        for(int IdxY = 0; IdxY <= SegmentsCount; ++IdxY)
        {
            const float SegmentY = (float)IdxY / (float)SegmentsCount;

            const SVectorFlt Loc = SVectorFlt(std::cos(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI),
                                              std::cos(SegmentY * M_PI),
                                              std::sin(SegmentX * 2.0f * M_PI) * std::sin(SegmentY * M_PI));

            Out.Vertices.emplace_back(Loc, Loc.GetNormalized(), Color);
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
                Out.Indices.push_back(IdxY * (SegmentsCount + 1) + IdxX);
                Out.Indices.push_back((IdxY + 1) * (SegmentsCount + 1) + IdxX);
            }
        }
        else
        {
            for(int IdxX = SegmentsCount; IdxX >= 0; --IdxX)
            {
                Out.Indices.push_back((IdxY + 1) * (SegmentsCount + 1) + IdxX);
                Out.Indices.push_back(IdxY * (SegmentsCount + 1) + IdxX);
            }
        }
        bOddRow = !bOddRow;
    }

    return Out;
}

SMeshDescription CreateTriangleMesh(const SLinearColor& Color)
{
    return {};
}

SMeshDescription CreateCircleMesh(const SLinearColor& Color)
{
    return {};
}

} // namespace South