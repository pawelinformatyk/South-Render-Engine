#include "Core/Shaders/Shader.h"

#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Shaders/ShadersLibrary.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace South
{

SShader::SShader(const VkDevice              Device,
                 const std::string&          inPathToCode,
                 const VkShaderStageFlagBits InStages,
                 const bool                  bCompile /*= true*/) : m_PathToCode(inPathToCode)
{
    m_ShaderInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .stage = InStages,
        .pName = "main",
    };

    if(bCompile)
    {
        Compile(Device);
    }
}

SShader::~SShader()
{
    const VkDevice LogDev = SRendererContext::Get().GetDeviceAndQueues().GetLogicalDevice();
    vkDestroyShaderModule(LogDev, m_ShaderInfo.module, nullptr);
}

void SShader::Compile(const VkDevice Device)
{
    const auto CurrentPath = std::filesystem::current_path();

    const std::ifstream Stream(m_PathToCode);
    std::stringstream   StrStream;
    StrStream << Stream.rdbuf();

    const std::string CodeAsString = StrStream.str();

    const shaderc::Compiler&       Compiler        = SShadersLibrary::GetCompiler();
    const shaderc::CompileOptions& CompilerOptions = SShadersLibrary::GetCompilerOptions();

    const auto ShaderKind = GetShadercShaderKind(m_ShaderInfo.stage);

    const shaderc::SpvCompilationResult Result = Compiler.CompileGlslToSpv(CodeAsString, ShaderKind, m_PathToCode.data(), CompilerOptions);

    if(Result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        STH_ERR(Result.GetErrorMessage());

        return;
    }

    const std::vector<uint32_t> SpirvCode(Result.cbegin(), Result.cend());
    // #TODO : Cache spirv in some directory.

    m_ShaderInfo.module = CreateShaderModule(Device, SpirvCode);
};

const VkPipelineShaderStageCreateInfo& SShader::GetInfo() const
{
    return m_ShaderInfo;
}

VkShaderModule SShader::CreateShaderModule(const VkDevice Device, const std::vector<uint32_t>& glslCode)
{
    VkShaderModule Module;

    const VkShaderModuleCreateInfo CreateInfo {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = nullptr,
        .codeSize = glslCode.size() * sizeof(uint32_t),
        .pCode    = glslCode.data(),
    };

    vkCreateShaderModule(Device, &CreateInfo, nullptr, &Module);

    return Module;
}

shaderc_shader_kind SShader::GetShadercShaderKind(const VkShaderStageFlagBits InStages)
{
    switch(InStages)
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