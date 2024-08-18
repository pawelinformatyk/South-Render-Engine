#pragma once

#include "Core/Buffers/VertexIndexBuffer.h"
#include "Core/Math/LinearColor.h"
#include "vulkan/vulkan_core.h"

namespace South
{

struct Vertex
{
    SVectorFlt   Location;
    SVectorFlt   Normal;
    SLinearColor Color;

    static const VkVertexInputBindingDescription&                  GetBindingDescription();
    static const std::array<VkVertexInputAttributeDescription, 3>& GetAttributesDescriptions();

private:
    static VkVertexInputBindingDescription                  m_BindingDesc;
    static std::array<VkVertexInputAttributeDescription, 3> m_AttributesDescs;
};

class SMesh
{
public:
    std::vector<Vertex>   Vertices;
    std::vector<uint32_t> Indices;

    // #TODO: For now raw ptr till I figure out render/scene relation
    SVertexIndexBuffer* Buffer;
};

struct SMeshDescription
{
    std::vector<Vertex>   Vertices;
    std::vector<uint32_t> Indices;
};

// Examples:

SMeshDescription CreatePlaneMesh(const SLinearColor& Color = SLinearColor::Rand());
SMeshDescription CreateCubeMesh(const SLinearColor& Color = SLinearColor::Rand());
SMeshDescription CreateSphereMesh(const SLinearColor& Color = SLinearColor::Rand(), int SegmentsCount = 16);
SMeshDescription CreateTriangleMesh(const SLinearColor& Color = SLinearColor::Rand());
SMeshDescription CreateCircleMesh(const SLinearColor& Color = SLinearColor::Rand());

} // namespace South