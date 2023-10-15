#include "sthpch.h"

#include "Core/Shaders/ShadersLibrary.h"

namespace South
{

namespace Private
{

std::string GetShaderFileExtension(const VkShaderStageFlagBits Stage)
{
    switch(Stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return ".vert";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return ".frag";
        default:
            return ".IDC";
    }
}

} // namespace Private

void ShadersLibrary::Init()
{
    s_CompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
    s_CompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

    STH_INFO("ShadersLibrary : Initialized.");
}

void ShadersLibrary::Deinit()
{
    for(const auto& [Name, Shader] : s_Shaders)
    {
        delete Shader;
    }

    s_Shaders.clear();

    STH_INFO("ShadersLibrary : Deinitialized");
}

Shader* ShadersLibrary::AddShader(const VkDevice Device, const std::string& Name, const VkShaderStageFlagBits Stage, const bool bCompile)
{
    const std::string Path = s_ShadersDirectory + Name + Private::GetShaderFileExtension(Stage);

    return AddShader(Device, Name, Path, Stage, bCompile);
}

Shader* ShadersLibrary::AddShader(const VkDevice              Device,
                                  const std::string&          Name,
                                  const std::string&          PathToCode,
                                  const VkShaderStageFlagBits Stage,
                                  const bool                  bCompile /*= true*/)
{
    auto* NewShader = new Shader(Device, PathToCode, Stage, bCompile);
    if(!NewShader)
    {
        return nullptr;
    }

    s_Shaders.emplace(Name, NewShader);

    STH_INFO("ShadersLibrary : {:s}-shader {:s} added.", GetShaderStageToString(Stage), Name);

    return NewShader;
}

Shader* ShadersLibrary::GetShader(const std::string& Name)
{
    return s_Shaders.contains(Name) ? s_Shaders[Name] : nullptr;
}

const std::unordered_map<std::string, Shader*>& ShadersLibrary::GetShaders()
{
    return s_Shaders;
}

shaderc::Compiler& ShadersLibrary::GetCompiler()
{
    return s_Compiler;
}

shaderc::CompileOptions& ShadersLibrary::GetCompilerOptions()
{
    return s_CompilerOptions;
}

const char* ShadersLibrary::GetShaderStageToString(const VkShaderStageFlagBits Stage)
{
    switch(Stage)
    {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "vertex";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "fragment";
        default:
            return "IDC";
    }
}

} // namespace South