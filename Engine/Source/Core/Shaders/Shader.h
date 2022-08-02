#pragma once

#include "shaderc/shaderc.hpp"
#include "vulkan/vulkan_core.h"

namespace South
{
    // #TODO : Shader should be aware of pushConstants, desriptors etc.? Look Record function in context.
    // #TODO : Cache compiled shaders (spirv) in some directory.
    class Shader
    {
    public:
        Shader(const std::string& inPathToCode, VkShaderStageFlagBits InStages, bool bCompile = true);
        ~Shader();

        void Compile();

        const VkPipelineShaderStageCreateInfo& GetInfo() const;

    private:
        VkShaderModule CreateShaderModule(const std::vector<uint32_t>& GlslCode);

        std::string m_PathToCode;

        VkPipelineShaderStageCreateInfo m_ShaderInfo;

        // Static functions
    public:
        // #TODO : Move to Utils
        static shaderc_shader_kind GetShadercShaderKind(VkShaderStageFlagBits InStages);
    };

} // namespace South