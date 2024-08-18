#pragma once

#include "Core/Buffers/VertexIndexBuffer.h"
#include "vulkan/vulkan_core.h"

namespace South
{

struct Vertex
{
    SVectorFlt Location;
    SVectorFlt Normal;
    SVectorFlt Color;

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

// Examples:

class SPlaneMesh : public SMesh
{
public:
    SPlaneMesh(const SVectorFlt& Color = SVectorFlt::Rand(0, 1));
};

class SCubeMesh : public SMesh
{
public:
    SCubeMesh(const SVectorFlt& Color = SVectorFlt::Rand(0, 1));
};

class SSphereMesh : public SMesh
{
public:
    SSphereMesh(const SVectorFlt& Color = SVectorFlt::Rand(0, 1), int SegmentsCount = 16);
};

class STriangleMesh : public SMesh
{
public:
    STriangleMesh(const SVectorFlt& Color = SVectorFlt::Rand(0, 1));
};

class SCircleMesh : public SMesh
{
public:
    SCircleMesh(const SVectorFlt& Color = SVectorFlt::Rand(0, 1));
};

} // namespace South