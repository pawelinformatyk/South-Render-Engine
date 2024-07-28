#pragma once

#include "vulkan/vulkan_core.h"
#include <Core/Buffers/VertexIndexBuffer.h>

namespace South
{


struct Vertex
{
    VectorFlt Pos;
    VectorFlt Normal;
    glm::vec2 TexCoords;
    VectorFlt Color;

    static const VkVertexInputBindingDescription&                  GetBindingDescription();
    static const std::array<VkVertexInputAttributeDescription, 4>& GetAttributesDescriptions();

private:
    static VkVertexInputBindingDescription                  m_BindingDesc;
    static std::array<VkVertexInputAttributeDescription, 4> m_AttributesDescs;
};

class Mesh
{
public:
    Mesh();

    std::vector<Vertex>   Vertices;
    std::vector<uint16_t> Indices;

    std::unique_ptr<VertexIndexBuffer> Buffer;
};

// Examples:

class PlaneMesh : public Mesh
{
public:
    PlaneMesh();
};

class CubeMesh : public Mesh
{
public:
    CubeMesh();
};

class SphereMesh : public Mesh
{
public:
    SphereMesh(int SegmentsCount);
};

class TriangleMesh : public Mesh
{
public:
    TriangleMesh();
};

class CircleMesh : public Mesh
{
public:
    CircleMesh();
};

} // namespace South