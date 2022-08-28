#include "sthpch.h"

#include "Core/Shaders/Shader.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Shaders/ShadersLibrary.h"
#include "Core/Devices/GraphicCard.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace South
{

    Shader::Shader(const std::string& inPathToCode, VkShaderStageFlagBits InStages, bool bCompile /*= true*/)
        : m_PathToCode(inPathToCode)
    {
        m_ShaderInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .stage = InStages,
            .pName = "main",
        };

        if (bCompile) { Compile(); }
    }

    Shader::~Shader()
    {
        VkDevice LogDev = Renderer::GetContext().GetLogicalDevice();
        vkDestroyShaderModule(LogDev, m_ShaderInfo.module, nullptr);
    }

    void Shader::Compile()
    {
        const std::ifstream Stream(m_PathToCode);
        std::stringstream StrStream;
        StrStream << Stream.rdbuf();

        const std::string GlslCode = StrStream.str();

        shaderc::Compiler& Compiler             = ShadersLibrary::GetCompiler();
        shaderc::CompileOptions CompilerOptions = ShadersLibrary::GetCompilerOptions();

        const auto ShaderKind = GetShadercShaderKind(m_ShaderInfo.stage);

        const shaderc::SpvCompilationResult Result =
            Compiler.CompileGlslToSpv(GlslCode, ShaderKind, m_PathToCode.data(), CompilerOptions);

        if (Result.GetCompilationStatus() != shaderc_compilation_status_success) { return; }

        const std::vector<uint32_t> SpirvCode(Result.cbegin(), Result.cend());
        // #TODO : Cache spirv in some directory.

        VkShaderModule Module = CreateShaderModule(SpirvCode);

        m_ShaderInfo.module = Module;
    };

    const VkPipelineShaderStageCreateInfo& Shader::GetInfo() const { return m_ShaderInfo; }

    VkShaderModule Shader::CreateShaderModule(const std::vector<uint32_t>& glslCode)
    {
        VkDevice LogDev = Renderer::GetContext().GetLogicalDevice();
        VkShaderModule Module;

        VkShaderModuleCreateInfo CreateInfo{
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .pNext    = nullptr,
            .codeSize = glslCode.size() * sizeof(uint32_t),
            .pCode    = glslCode.data(),
        };

        vkCreateShaderModule(LogDev, &CreateInfo, nullptr, &Module);

        return Module;
    }

    shaderc_shader_kind Shader::GetShadercShaderKind(VkShaderStageFlagBits InStages)
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
