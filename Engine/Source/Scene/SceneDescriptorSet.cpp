#include "SceneDescriptorSet.h"

#include "Core/Renderer/RendererContext.h"

namespace South
{

void SSceneDescriptorSet::UpdateSets(const VkDescriptorBufferInfo& CameraBufferInfo, const VkDescriptorImageInfo& CombinedImageInfo) const
{
    std::array<VkWriteDescriptorSet, 2> descriptorWrites {};

    descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet          = Set;
    descriptorWrites[0].dstBinding      = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo     = &CameraBufferInfo;

    descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet          = Set;
    descriptorWrites[1].dstBinding      = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo      = &CombinedImageInfo;

    vkUpdateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
                           descriptorWrites.size(),
                           descriptorWrites.data(),
                           0,
                           nullptr);
}
SSceneDescriptorSet::SSceneDescriptorSet()
{
    constexpr VkDescriptorSetLayoutBinding UboLayoutBinding {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = nullptr,
    };

    // #TODO: Sampler texture in fragment shader

    constexpr std::array Bindings = {UboLayoutBinding};

    const VkDescriptorSetLayoutCreateInfo LayoutCi {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pNext        = nullptr,
        .bindingCount = static_cast<uint32_t>(Bindings.size()),
        .pBindings    = Bindings.data(),
    };
    vkCreateDescriptorSetLayout(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &LayoutCi, nullptr, &Layout);

    // #TODO: How many PoolSizes? And how many sets.
    const std::vector<VkDescriptorPoolSize> PoolSizes = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                                         {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

    const VkDescriptorPoolCreateInfo PoolCi = {
        .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .pNext         = nullptr,
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets       = 1000 * static_cast<uint32_t>(PoolSizes.size()),
        .poolSizeCount = static_cast<uint32_t>(PoolSizes.size()),
        .pPoolSizes    = PoolSizes.data(),
    };
    vkCreateDescriptorPool(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &PoolCi, nullptr, &Pool);

    const VkDescriptorSetAllocateInfo SetCi {
        .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .pNext              = nullptr,
        .descriptorPool     = Pool,
        .descriptorSetCount = 1,
        .pSetLayouts        = &Layout,
    };

    vkAllocateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &SetCi, &Set);
}

SSceneDescriptorSet::~SSceneDescriptorSet()
{
    // vkDestroyDescriptorSetLayout(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), DescriptorSetLayout, nullptr);
}

VkDescriptorSetLayout SSceneDescriptorSet::GetLayout() const
{
    return {Layout};
}

} // namespace South