#include "sthpch.h"

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

std::array<VkVertexInputAttributeDescription, 3> Vertex::m_AttributesDescs {
    VkVertexInputAttributeDescription {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, m_Pos),
    },
    VkVertexInputAttributeDescription {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = offsetof(Vertex, m_Normal),
    },
    VkVertexInputAttributeDescription {
        .location = 2,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = offsetof(Vertex, m_TexCoords),
    },
};

} // namespace South