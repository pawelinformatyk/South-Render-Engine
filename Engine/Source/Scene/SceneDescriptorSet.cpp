#include "SceneDescriptorSet.h"

#include "Core/Renderer/RendererContext.h"

namespace South
{

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
        .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .maxSets       = 1000 * static_cast<uint32_t>(PoolSizes.size()),
        .poolSizeCount = static_cast<uint32_t>(PoolSizes.size()),
        .pPoolSizes    = PoolSizes.data(),
    };
    vkCreateDescriptorPool(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &PoolCi, nullptr, &Pool);
    //
    // std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, Layout);
    // VkDescriptorSetAllocateInfo        allocInfo {};
    // allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    // allocInfo.descriptorPool     = Pool;
    // allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    // allocInfo.pSetLayouts        = layouts.data();
    //
    // Sets.resize(MAX_FRAMES_IN_FLIGHT);
    // if(vkAllocateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(), &allocInfo, Sets.data()) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to allocate descriptor sets!");
    // }
    //
    // for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    // {
    //     VkDescriptorBufferInfo bufferInfo {};
    //     bufferInfo.buffer = m_CameraUbos[i]->GetBuffer();
    //     bufferInfo.offset = 0;
    //     bufferInfo.range  = sizeof(UniformBufferObject);
    //
    //     VkDescriptorImageInfo imageInfo {};
    //     imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    //     imageInfo.imageView   = m_SceneTextureImageView;
    //     imageInfo.sampler     = m_TextureSampler;
    //
    //     std::array<VkWriteDescriptorSet, 2> descriptorWrites {};
    //
    //     descriptorWrites[0].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    //     descriptorWrites[0].dstSet          = Sets[i];
    //     descriptorWrites[0].dstBinding      = 0;
    //     descriptorWrites[0].dstArrayElement = 0;
    //     descriptorWrites[0].descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //     descriptorWrites[0].descriptorCount = 1;
    //     descriptorWrites[0].pBufferInfo     = &bufferInfo;
    //
    //     descriptorWrites[1].sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    //     descriptorWrites[1].dstSet          = Sets[i];
    //     descriptorWrites[1].dstBinding      = 1;
    //     descriptorWrites[1].dstArrayElement = 0;
    //     descriptorWrites[1].descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //     descriptorWrites[1].descriptorCount = 1;
    //     descriptorWrites[1].pImageInfo      = &imageInfo;
    //
    //     vkUpdateDescriptorSets(SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice(),
    //                            static_cast<uint32_t>(descriptorWrites.size()),
    //                            descriptorWrites.data(),
    //                            0,
    //                            nullptr);
    // }
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