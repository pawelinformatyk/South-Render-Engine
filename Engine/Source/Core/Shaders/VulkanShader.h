#pragma once

#include "vulkan/vulkan_core.h"
#include "shaderc/shaderc.hpp"

namespace South
{
    // #TODO : Shader should be aware of pushConstants, desriptors etc.? Look Record function in context.
    // #TODO : Cache compiled shaders in some directory.
    class VulkanShader
    {
      public:
        VulkanShader(const std::string& inPathToCode, VkShaderStageFlagBits InStages, bool bCompile = true);
        ~VulkanShader();

        void Compile();

        const VkPipelineShaderStageCreateInfo& GetInfo() const;

      private:
        VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

        std::string pathToCode;

        VkPipelineShaderStageCreateInfo info;

        // Static functions
      public:
        // #TODO : Move to Utils
        static shaderc_shader_kind GetShadercShaderKind(VkShaderStageFlagBits InStages);
    };

} // namespace South
