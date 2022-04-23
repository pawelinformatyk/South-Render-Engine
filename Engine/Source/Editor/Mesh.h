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
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 color;

        static const VkVertexInputBindingDescription& GetBindingDescription();
        static const std::array<VkVertexInputAttributeDescription, 4>& GetAttributesDescriptions();

      private:
        static VkVertexInputBindingDescription bindingDesc;
        static std::array<VkVertexInputAttributeDescription, 4> attributesDescs;
    };

    // #TODO : One/two/three... meshes - one/two buffer
    class StaticMesh
    {
      public:
        StaticMesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices);

      private:
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        // VulkanIndexBuffer* indexBuffer;
        // VulkanVertexBuffer* verticesBuffer;
        SVulkanVertexIndexBuffer* VI_Buffer;
    };


} // namespace South