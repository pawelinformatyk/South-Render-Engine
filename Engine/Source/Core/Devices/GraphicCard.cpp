#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Utils/VulkanUtils.h"
#include <optional>

namespace South
{
namespace Private
{

bool CheckFeatures(VkPhysicalDevice InPhysDev, const VkPhysicalDeviceFeatures& InRequiredFeatures);
bool CheckExtensions(VkPhysicalDevice InPhysDev, const std::vector<const char*>& InRequiredExtensions);

VkPhysicalDevice FindPhysicalDevice(const VkInstance                InVulkanInstance,
                                    const std::vector<const char*>& InRequiredExtensions,
                                    const VkPhysicalDeviceFeatures& InRequiredFeatures)
{
    uint32_t GpuCount = 0;
    vkEnumeratePhysicalDevices(InVulkanInstance, &GpuCount, nullptr);

    std::vector<VkPhysicalDevice> AvailableGpus(GpuCount);
    vkEnumeratePhysicalDevices(InVulkanInstance, &GpuCount, AvailableGpus.data());

    for(const VkPhysicalDevice Gpu : AvailableGpus)
    {
        if(!CheckFeatures(Gpu, InRequiredFeatures))
            continue;

        if(!CheckExtensions(Gpu, InRequiredExtensions))
            continue;

        return Gpu;
    }

    return nullptr;
}

bool CheckFeatures(const VkPhysicalDevice InPhysDev, const VkPhysicalDeviceFeatures& InRequiredFeatures)
{
    VkPhysicalDeviceFeatures FoundFeatures;
    vkGetPhysicalDeviceFeatures(InPhysDev, &FoundFeatures);

    auto IsFeatureNotPresent = [](const bool InLhs, const bool InRhs)
    {
        return InLhs && InLhs != InRhs;
    };

    if(IsFeatureNotPresent(InRequiredFeatures.robustBufferAccess, FoundFeatures.robustBufferAccess))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.fullDrawIndexUint32, FoundFeatures.fullDrawIndexUint32))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.fullDrawIndexUint32, FoundFeatures.fullDrawIndexUint32))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.imageCubeArray, FoundFeatures.imageCubeArray))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.independentBlend, FoundFeatures.independentBlend))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.geometryShader, FoundFeatures.geometryShader))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.tessellationShader, FoundFeatures.tessellationShader))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sampleRateShading, FoundFeatures.sampleRateShading))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.dualSrcBlend, FoundFeatures.dualSrcBlend))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.logicOp, FoundFeatures.logicOp))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.multiDrawIndirect, FoundFeatures.multiDrawIndirect))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.drawIndirectFirstInstance, FoundFeatures.drawIndirectFirstInstance))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.depthClamp, FoundFeatures.depthClamp))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.depthBiasClamp, FoundFeatures.depthBiasClamp))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.fillModeNonSolid, FoundFeatures.fillModeNonSolid))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.depthBounds, FoundFeatures.depthBounds))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.wideLines, FoundFeatures.wideLines))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.largePoints, FoundFeatures.largePoints))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.alphaToOne, FoundFeatures.alphaToOne))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.multiViewport, FoundFeatures.multiViewport))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.samplerAnisotropy, FoundFeatures.samplerAnisotropy))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.textureCompressionETC2, FoundFeatures.textureCompressionETC2))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.textureCompressionASTC_LDR, FoundFeatures.textureCompressionASTC_LDR))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.textureCompressionBC, FoundFeatures.textureCompressionBC))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.occlusionQueryPrecise, FoundFeatures.occlusionQueryPrecise))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.pipelineStatisticsQuery, FoundFeatures.pipelineStatisticsQuery))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.vertexPipelineStoresAndAtomics, FoundFeatures.vertexPipelineStoresAndAtomics))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.fragmentStoresAndAtomics, FoundFeatures.fragmentStoresAndAtomics))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderTessellationAndGeometryPointSize, FoundFeatures.shaderTessellationAndGeometryPointSize))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderImageGatherExtended, FoundFeatures.shaderImageGatherExtended))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageExtendedFormats, FoundFeatures.shaderStorageImageExtendedFormats))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageMultisample, FoundFeatures.shaderStorageImageMultisample))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageReadWithoutFormat, FoundFeatures.shaderStorageImageReadWithoutFormat))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageWriteWithoutFormat, FoundFeatures.shaderStorageImageWriteWithoutFormat))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderUniformBufferArrayDynamicIndexing,
                           FoundFeatures.shaderUniformBufferArrayDynamicIndexing))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderSampledImageArrayDynamicIndexing, FoundFeatures.shaderSampledImageArrayDynamicIndexing))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageBufferArrayDynamicIndexing,
                           FoundFeatures.shaderStorageBufferArrayDynamicIndexing))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderStorageImageArrayDynamicIndexing, FoundFeatures.shaderStorageImageArrayDynamicIndexing))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderClipDistance, FoundFeatures.shaderClipDistance))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderCullDistance, FoundFeatures.shaderCullDistance))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderFloat64, FoundFeatures.shaderFloat64))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderInt64, FoundFeatures.shaderInt64))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderInt16, FoundFeatures.shaderInt16))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderResourceResidency, FoundFeatures.shaderResourceResidency))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.shaderResourceMinLod, FoundFeatures.shaderResourceMinLod))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseBinding, FoundFeatures.sparseBinding))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidencyBuffer, FoundFeatures.sparseResidencyBuffer))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidencyImage2D, FoundFeatures.sparseResidencyImage2D))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidencyImage3D, FoundFeatures.sparseResidencyImage3D))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidency2Samples, FoundFeatures.sparseResidency2Samples))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidency4Samples, FoundFeatures.sparseResidency4Samples))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidency8Samples, FoundFeatures.sparseResidency8Samples))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidency16Samples, FoundFeatures.sparseResidency16Samples))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.sparseResidencyAliased, FoundFeatures.sparseResidencyAliased))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.variableMultisampleRate, FoundFeatures.variableMultisampleRate))
        return false;
    if(IsFeatureNotPresent(InRequiredFeatures.inheritedQueries, FoundFeatures.inheritedQueries))
        return false;

    return true;
}

bool CheckExtensions(const VkPhysicalDevice InPhysDev, const std::vector<const char*>& InRequiredExtensions)
{
    uint32_t ExtensionCount;
    vkEnumerateDeviceExtensionProperties(InPhysDev, nullptr, &ExtensionCount, nullptr);

    std::vector<VkExtensionProperties> Extensions = std::vector<VkExtensionProperties>(ExtensionCount);
    vkEnumerateDeviceExtensionProperties(InPhysDev, nullptr, &ExtensionCount, Extensions.data());

    std::set<std::string> RequiredExtensionsSet(InRequiredExtensions.begin(), InRequiredExtensions.end());

    for(const VkExtensionProperties& Extension : Extensions)
    {
        RequiredExtensionsSet.erase(Extension.extensionName);
    }

    if(!RequiredExtensionsSet.empty())
    {
        return false;
    }

    return true;
}

std::optional<std::pair<uint32_t, uint32_t>> FindGraphicAndPresentQueueFamilyIndex(const VkPhysicalDevice InPhysDev,
                                                                                   const VkSurfaceKHR     InSurface)
{
    uint32_t QueueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(InPhysDev, &QueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> QueueFamilies(QueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(InPhysDev, &QueueFamilyCount, QueueFamilies.data());

    std::optional<uint32_t> GraphicIndex {};

    for(uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
    {
        if(QueueFamilies[QueueFamilyIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            GraphicIndex = QueueFamilyIndex;

            break;
        }
    }

    if(!GraphicIndex.has_value())
    {
        return {};
    }

    std::optional<uint32_t> PresentIndex {};

    for(uint32_t QueueFamilyIndex = 0; QueueFamilyIndex < QueueFamilyCount; ++QueueFamilyIndex)
    {
        if(GraphicIndex.value() == QueueFamilyIndex)
        {
            continue;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(InPhysDev, QueueFamilyIndex, InSurface, &presentSupport);

        if(presentSupport)
        {
            PresentIndex = QueueFamilyIndex;

            break;
        }
    }

    if(PresentIndex.has_value())
    {
        return std::make_pair(GraphicIndex.value(), PresentIndex.value());
    }

    return std::make_pair(GraphicIndex.value(), GraphicIndex.value());
}

} // namespace Private

SGraphicCard* SGraphicCard::Create(const VkInstance InVulkanInstance, const SCreateInfo& InCreateInfo)
{
    const VkPhysicalDevice FoundDevice =
        Private::FindPhysicalDevice(InVulkanInstance, InCreateInfo.RequiredExtensions, InCreateInfo.RequiredFeatures);
    if(!FoundDevice)
    {
        return nullptr;
    }

    auto* OutGpu = new SGraphicCard();

    OutGpu->m_PhysicalDevice  = FoundDevice;
    OutGpu->m_ExtensionsNames = InCreateInfo.RequiredExtensions;
    vkGetPhysicalDeviceFeatures(FoundDevice, &OutGpu->m_Features);
    vkGetPhysicalDeviceProperties(FoundDevice, &OutGpu->m_Properties);
    OutGpu->m_TypeName = VulkanUtils::DeviceTypeToString(OutGpu->m_Properties.deviceType);

    return OutGpu;
}

VkPhysicalDevice SGraphicCard::GetPhysicalDevice() const
{
    return m_PhysicalDevice;
}

const std::vector<const char*>& SGraphicCard::GetExtensionsNames() const
{
    return m_ExtensionsNames;
}

const VkPhysicalDeviceFeatures& SGraphicCard::GetFeatures() const
{
    return m_Features;
}

const VkPhysicalDeviceProperties& SGraphicCard::GetProperties() const
{
    return m_Properties;
}

std::string_view SGraphicCard::GetTypeName() const
{
    return m_TypeName;
}

SLogicalDeviceAndQueues* SLogicalDeviceAndQueues::Create(const SGraphicCard& InGpu, const VkSurfaceKHR InSurface)
{
    const VkPhysicalDevice PhysDev = InGpu.GetPhysicalDevice();

    const std::optional<std::pair<uint32_t, uint32_t>> GraphicAndPresentQueueFamilyIndex =
        Private::FindGraphicAndPresentQueueFamilyIndex(PhysDev, InSurface);
    if(!GraphicAndPresentQueueFamilyIndex.has_value())
    {
        STH_VK_ERR("GraphicAndPresentQueueFamilyIndex not found.");
        return nullptr;
    }

    const std::array<float, 2> QueuePrios = {1.f, 1.f};

    std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

    const VkDeviceQueueCreateInfo GraphicQueueInfo {
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext            = nullptr,
        .queueFamilyIndex = GraphicAndPresentQueueFamilyIndex->first,
        .queueCount       = 1,
        .pQueuePriorities = &QueuePrios[0],
    };

    QueueCreateInfos.emplace_back(GraphicQueueInfo);

    // Independent Present and graphic queues.
    if(GraphicAndPresentQueueFamilyIndex->first != GraphicAndPresentQueueFamilyIndex->second)
    {
        const VkDeviceQueueCreateInfo PresentQueueInfo {
            .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext            = nullptr,
            .queueFamilyIndex = GraphicAndPresentQueueFamilyIndex->second,
            .queueCount       = 1,
            .pQueuePriorities = &QueuePrios[1],
        };

        QueueCreateInfos.emplace_back(PresentQueueInfo);
    }

    const VkDeviceCreateInfo LogicDeviceCreateInfo {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext                   = nullptr,
        .flags                   = 0x00,
        .queueCreateInfoCount    = static_cast<uint32_t>(QueueCreateInfos.size()),
        .pQueueCreateInfos       = QueueCreateInfos.data(),
        .enabledExtensionCount   = static_cast<uint32_t>(InGpu.GetExtensionsNames().size()),
        .ppEnabledExtensionNames = InGpu.GetExtensionsNames().data(),
        .pEnabledFeatures        = &InGpu.GetFeatures(),
    };

    auto* OutDevice = new SLogicalDeviceAndQueues();

    OutDevice->m_GraphicQueueFamilyIndex = GraphicAndPresentQueueFamilyIndex->first;
    OutDevice->m_PresentQueueFamilyIndex = GraphicAndPresentQueueFamilyIndex->second;

    vkCreateDevice(PhysDev, &LogicDeviceCreateInfo, nullptr, &OutDevice->m_LogicalDevice);

    vkGetDeviceQueue(OutDevice->m_LogicalDevice, OutDevice->m_GraphicQueueFamilyIndex, 0, &OutDevice->m_GraphicQueue);

    // Independent Present and graphic queues.
    if(GraphicAndPresentQueueFamilyIndex->first != GraphicAndPresentQueueFamilyIndex->second)
    {
        vkGetDeviceQueue(OutDevice->m_LogicalDevice, OutDevice->m_PresentQueueFamilyIndex, 0, &OutDevice->m_PresentQueue);
    }
    else
    {
        OutDevice->m_PresentQueue = OutDevice->m_GraphicQueue;
    }

    return OutDevice;
}

void SLogicalDeviceAndQueues::Destroy(SLogicalDeviceAndQueues& InDeviceAndQueues)
{
    vkDestroyDevice(InDeviceAndQueues.m_LogicalDevice, nullptr);
    InDeviceAndQueues.m_LogicalDevice = nullptr;
    InDeviceAndQueues.m_GraphicQueue  = nullptr;
    InDeviceAndQueues.m_PresentQueue  = nullptr;
}

VkDevice SLogicalDeviceAndQueues::GetLogicalDevice() const
{
    return m_LogicalDevice;
}

VkQueue SLogicalDeviceAndQueues::GetGraphicQueue() const
{
    return m_GraphicQueue;
}

uint32_t SLogicalDeviceAndQueues::GetGraphicQueueFamilyIndex() const
{
    return m_GraphicQueueFamilyIndex;
}

VkQueue SLogicalDeviceAndQueues::GetPresentQueue() const
{
    return m_PresentQueue;
}

uint32_t SLogicalDeviceAndQueues::GetPresentQueueFamilyIndex() const
{
    return m_PresentQueueFamilyIndex;
}

} // namespace South