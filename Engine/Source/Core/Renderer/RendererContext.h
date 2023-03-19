#pragma once

#include "glm.hpp"
#include "vulkan/vulkan_core.h"

struct GLFWwindow;

namespace South
{

class GraphicCard;
class LogicalDeviceAndQueues;

struct PushConstant
{
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;
};

// Class holding all "global" vulkan related variables.
class RendererContext
{
    friend class Renderer;

public:
    void Init();
    void DeInit();

    VkInstance         GetVulkanInstance() const;
    VkQueue            GetGraphicQueue() const;
    uint32_t           GetGraphicQueueFamilyIndex() const;
    VkDevice           GetLogicalDevice() const;
    const GraphicCard& GetGraphicCard() const;

private:
    void CreateInstance();

    void CreateSurface(GLFWwindow& InWindow);

    VkInstance m_VulkanInstance = nullptr;

    GraphicCard*            m_GPU           = nullptr;
    LogicalDeviceAndQueues* m_LogicalDevice = nullptr;

    //~ Validations layers.
private:
    void CreateMessenger();
    void DestroyMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      InMessageSeverity,
                                                                    VkDebugUtilsMessageTypeFlagsEXT             InMessageType,
                                                                    const VkDebugUtilsMessengerCallbackDataEXT* InCallbackData,
                                                                    void*                                       InUserData);

    bool CheckValidationLayers() const;

    const std::vector<const char*> m_RequiredValidationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool m_bEnableValidationLayers = false;
#else
    const bool m_bEnableValidationLayers = true;
#endif

    VkDebugUtilsMessengerEXT m_Messenger;
    //~ Validations layers.
};
}; // namespace South