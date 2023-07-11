#pragma once

#include "glm.hpp"
#include "vulkan/vulkan_core.h"

namespace South
{

class VertexIndexBuffer;
class VertexBuffer;
class SVulkanVertexIndexBuffer;

struct Vertex
{
    VectorF   Pos;
    VectorF   Normal;
    glm::vec2 TexCoords;

    static const VkVertexInputBindingDescription&                  GetBindingDescription();
    static const std::array<VkVertexInputAttributeDescription, 3>& GetAttributesDescriptions();

private:
    static VkVertexInputBindingDescription                  m_BindingDesc;
    static std::array<VkVertexInputAttributeDescription, 3> m_AttributesDescs;
};

// #TODO : One/two/three... meshes - one/two buffer
class StaticMesh
{
public:
    StaticMesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndices);

private:
    std::vector<Vertex>   m_Vertices;
    std::vector<uint16_t> m_Indices;

    // VertexIndexBuffer* indexBuffer;
    // VertexBuffer* verticesBuffer;
    SVulkanVertexIndexBuffer* m_VI_Buffer;
};

} // namespace South