#pragma once

#include "Core/Buffers/VertexIndexBuffer.h"
#include "vulkan/vulkan_core.h"

namespace South
{


struct Vertex
{
    SVectorFlt Pos;
    SVectorFlt Normal;
    glm::vec2  TexCoords;
    SVectorFlt Color;

    static const VkVertexInputBindingDescription&                  GetBindingDescription();
    static const std::array<VkVertexInputAttributeDescription, 4>& GetAttributesDescriptions();

private:
    static VkVertexInputBindingDescription                  m_BindingDesc;
    static std::array<VkVertexInputAttributeDescription, 4> m_AttributesDescs;
};

class SMesh
{
public:
    SMesh();

    std::vector<Vertex>   Vertices;
    std::vector<uint16_t> Indices;

    std::unique_ptr<SVertexIndexBuffer> Buffer;
};

// Examples:

class SPlaneMesh : public SMesh
{
public:
    SPlaneMesh();
};

class SCubeMesh : public SMesh
{
public:
    SCubeMesh();
};

class SSphereMesh : public SMesh
{
public:
    SSphereMesh(int SegmentsCount);
};

class STriangleMesh : public SMesh
{
public:
    STriangleMesh();
};

class SCircleMesh : public SMesh
{
public:
    SCircleMesh();
};

} // namespace South