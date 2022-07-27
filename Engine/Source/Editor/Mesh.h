#pragma once

#include "vulkan/vulkan_core.h"
#include "glm.hpp"

namespace South
{
    class VulkanIndexBuffer;
    class VulkanVertexBuffer;
    class SVulkanVertexIndexBuffer;

    struct Vertex
    {
        glm::vec3 m_Pos;
        glm::vec3 m_Normal;
        glm::vec2 m_TexCoords;
        glm::vec3 m_Color;

        static const VkVertexInputBindingDescription& GetBindingDescription();
        static const std::array<VkVertexInputAttributeDescription, 4>& GetAttributesDescriptions();

    private:
        static VkVertexInputBindingDescription m_BindingDesc;
        static std::array<VkVertexInputAttributeDescription, 4> m_AttributesDescs;
    };

    // #TODO : One/two/three... meshes - one/two buffer
    class StaticMesh
    {
    public:
        StaticMesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices);

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint16_t> m_Indices;

        // VulkanIndexBuffer* indexBuffer;
        // VulkanVertexBuffer* verticesBuffer;
        SVulkanVertexIndexBuffer* m_VI_Buffer;
    };


} // namespace South