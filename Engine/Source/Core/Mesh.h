#pragma once

#include "vulkan/vulkan_core.h"
#include "glm.hpp"

namespace South
{
    class VulkanIndexBuffer;
    class VulkanVertexBuffer;

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

    class StaticMesh
    {
      public:
        StaticMesh(const std::vector<Vertex>& inVertices, const std::vector<uint16_t>& inIndices);

      private:
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        // #TODO : Where to put them? Should be in the same place as vectors?
        VulkanIndexBuffer* indexBuffer;
        VulkanVertexBuffer* verticesBuffer;
    };

    // Visible/rendered object?
    // #TODO : Should be passed to rendering pipeline?
    class Object
    {
      private:
        glm::mat4 Transform;

        StaticMesh* mesh = nullptr;
    };

} // namespace South