#pragma once
#include <vulkan/vulkan_core.h>

namespace South
{

class SSceneDescriptorSet
{
public:
     SSceneDescriptorSet();
    ~SSceneDescriptorSet();

    void UpdateSets(const VkDescriptorBufferInfo& CameraBufferInfo, const VkDescriptorImageInfo& CombinedImageInfo) const;

    VkDescriptorSetLayout GetLayout() const;

protected:
    VkDescriptorSetLayout Layout = nullptr;
    VkDescriptorPool      Pool   = nullptr;
    VkDescriptorSet       Set    = nullptr;
};

} // namespace South