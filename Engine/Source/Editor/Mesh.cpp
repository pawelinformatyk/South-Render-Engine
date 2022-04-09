#include "sthpch.h"

#include "Mesh.h"

namespace South
{

    const VkVertexInputBindingDescription& Vertex::GetBindingDescription()
    {
        return bindingDesc;
    }

    const std::array<VkVertexInputAttributeDescription, 4>& Vertex::GetAttributesDescriptions()
    {
        return attributesDescs;
    }

    VkVertexInputBindingDescription Vertex::bindingDesc{
        .binding   = 0,
        .stride    = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::array<VkVertexInputAttributeDescription, 4> Vertex::attributesDescs{
        VkVertexInputAttributeDescription{
            .location = 0,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, pos),
        },
        VkVertexInputAttributeDescription{
            .location = 1,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, normal),
        },
        VkVertexInputAttributeDescription{
            .location = 2,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32_SFLOAT,
            .offset   = offsetof(Vertex, texCoords),
        },
        VkVertexInputAttributeDescription{
            .location = 3,
            .binding  = 0,
            .format   = VK_FORMAT_R32G32B32_SFLOAT,
            .offset   = offsetof(Vertex, color),
        },
    };

} // namespace South