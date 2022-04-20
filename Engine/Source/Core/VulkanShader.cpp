#include "sthpch.h"

#include "Core/VulkanShader.h"
#include "Core/VulkanContext.h"
#include "Core/VulkanDevice.h"

#include <fstream>
#include <filesystem>

namespace South
{

    VulkanShader::VulkanShader(const std::string& inName, const std::string& inPathToSPIRV, ShaderType inType)
        : name(inName)
        , pathToSPIRV(inPathToSPIRV)
        , type(inType)
    {
        // #TODO : Check errors.
        std::vector<char> shaderCode = ReadFile(inPathToSPIRV);

        CreateShaderModule(shaderCode);

        VkShaderStageFlagBits shaderStage;
        switch (type)
        {
            case ShaderType::Vertex:
            {
                shaderStage = VK_SHADER_STAGE_VERTEX_BIT;
                break;
            }
            case ShaderType::Fragment:
            {
                shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT;
                break;
            }
        }

        info = {
            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext  = nullptr,
            .stage  = shaderStage,
            .module = module,
            .pName  = "main",
        };
    }

    VulkanShader::~VulkanShader()
    {
        VkDevice device = VulkanContext::Get().GetCurrentDevice().GetDevice();
        vkDestroyShaderModule(device, module, nullptr);
    }

    const VkPipelineShaderStageCreateInfo& VulkanShader::GetInfo() const
    {
        return info;
    }

    void VulkanShader::CreateShaderModule(const std::vector<char>& code)
    {
        VkDevice device = VulkanContext::Get().GetCurrentDevice().GetDevice();

        VkShaderModuleCreateInfo createInfo{
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext    = nullptr,
            .codeSize = code.size(),
            .pCode    = reinterpret_cast<const uint32_t*>(code.data()),
        };

        vkCreateShaderModule(device, &createInfo, nullptr, &module);
    }

    std::vector<char> VulkanShader::ReadFile(const std::string& fileName)
    {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        if (file.fail() || !file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();

        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

} // namespace South
