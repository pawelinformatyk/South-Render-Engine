#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{
    // #TODO : Redundant. StageFlag is enough.
    enum class ShaderType
    {
        Vertex,
        Fragment,
    };

    class VulkanShader
    {
      public:
        VulkanShader(const std::string& inName, const std::string& inPathToSPIRV, ShaderType inType);
        ~VulkanShader();

        const VkPipelineShaderStageCreateInfo& GetInfo() const;

      private:
        void CreateShaderModule(const std::vector<char>& code);
        std::vector<char> ReadFile(const std::string& fileName);

        std::string name;
        std::string pathToSPIRV;
        ShaderType type;

        VkShaderModule module;

        VkPipelineShaderStageCreateInfo info;
    };

} // namespace South
