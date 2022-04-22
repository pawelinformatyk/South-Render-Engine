#include "sthpch.h"

#include "Core/VulkanShader.h"

#include "Core/ShadersLibrary.h"
#include "Core/VulkanContext.h"
#include "Core/VulkanDevice.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace South
{

    VulkanShader::VulkanShader(const std::string& inPathToCode, VkShaderStageFlagBits InStages,
                               bool bCompile /*= true*/)
        : pathToCode(inPathToCode)
    {
        info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .stage = InStages,
            .pName = "main",
        };

        if (bCompile)
        {
            Compile();
        }
    }

    VulkanShader::~VulkanShader()
    {
        VkDevice device = VulkanContext::Get().GetCurrentDevice().GetDevice();
        vkDestroyShaderModule(device, info.module, nullptr);
    }

    void VulkanShader::Compile()
    {
        const std::ifstream stream(pathToCode);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        const std::string code = strStream.str();

        shaderc::Compiler& compiler             = ShadersLibrary::GetCompiler();
        shaderc::CompileOptions compilerOptions = ShadersLibrary::GetCompilerOptions();

        const auto shaderKind = GetShadercShaderKind(info.stage);

        const shaderc::SpvCompilationResult result =
            compiler.CompileGlslToSpv(code, shaderKind, pathToCode.data(), compilerOptions);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            return;
        }

        VkShaderModule module = CreateShaderModule(std::vector<uint32_t>(result.cbegin(), result.cend()));

        info.module = module;
    };

    const VkPipelineShaderStageCreateInfo& VulkanShader::GetInfo() const
    {
        return info;
    }

    VkShaderModule VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code)
    {
        VkDevice device = VulkanContext::Get().GetCurrentDevice().GetDevice();
        VkShaderModule module;

        VkShaderModuleCreateInfo createInfo{
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext    = nullptr,
            .codeSize = code.size() * sizeof(uint32_t),
            .pCode    = code.data(),
        };

        vkCreateShaderModule(device, &createInfo, nullptr, &module);

        return module;
    }

    shaderc_shader_kind VulkanShader::GetShadercShaderKind(VkShaderStageFlagBits InStages)
    {
        switch (InStages)
        {
            case VK_SHADER_STAGE_VERTEX_BIT:
                return shaderc_shader_kind::shaderc_vertex_shader;
            case VK_SHADER_STAGE_FRAGMENT_BIT:
                return shaderc_shader_kind::shaderc_fragment_shader;
            case VK_SHADER_STAGE_COMPUTE_BIT:
                return shaderc_shader_kind::shaderc_compute_shader;
        }

        return shaderc_shader_kind::shaderc_glsl_infer_from_source;
    }

} // namespace South
