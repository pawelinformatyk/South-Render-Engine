#pragma once
#include <vulkan/vulkan_core.h>

namespace South
{

class SSceneDescriptorSet
{
public:
     SSceneDescriptorSet();
    ~SSceneDescriptorSet();

    VkDescriptorSetLayout GetLayout() const;

protected:
    VkDescriptorSetLayout        Layout = nullptr;
    VkDescriptorPool             Pool   = nullptr;
    std::vector<VkDescriptorSet> Sets;
};

} // namespace South