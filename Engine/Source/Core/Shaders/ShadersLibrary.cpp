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

void SShadersLibrary::Init()
{
    s_CompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
    s_CompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

    STH_INFO("ShadersLibrary : Initialized.");
}

void SShadersLibrary::Deinit()
{
    for(const auto& [Name, Shader] : s_Shaders)
    {
        delete Shader;
    }

    s_Shaders.clear();

    STH_INFO("ShadersLibrary : Deinitialized");
}

SShader* SShadersLibrary::AddShader(const VkDevice Device, const std::string& Name, const VkShaderStageFlagBits Stage, const bool bCompile)
{
    const std::string Path = s_ShadersDirectory + Name + Private::GetShaderFileExtension(Stage);

    return AddShader(Device, Name, Path, Stage, bCompile);
}

SShader* SShadersLibrary::AddShader(const VkDevice              Device,
                                  const std::string&          Name,
                                  const std::string&          PathToCode,
                                  const VkShaderStageFlagBits Stage,
                                  const bool                  bCompile /*= true*/)
{
    auto* NewShader = new SShader(Device, PathToCode, Stage, bCompile);
    if(!NewShader)
    {
        return nullptr;
    }

    s_Shaders.emplace(Name, NewShader);

    STH_INFO("ShadersLibrary : {:s}-shader {:s} added.", GetShaderStageToString(Stage), Name);

    return NewShader;
}

SShader* SShadersLibrary::GetShader(const std::string& Name)
{
    return s_Shaders.contains(Name) ? s_Shaders[Name] : nullptr;
}

const std::unordered_map<std::string, SShader*>& SShadersLibrary::GetShaders()
{
    return s_Shaders;
}

shaderc::Compiler& SShadersLibrary::GetCompiler()
{
    return s_Compiler;
}

shaderc::CompileOptions& SShadersLibrary::GetCompilerOptions()
{
    return s_CompilerOptions;
}

const char* SShadersLibrary::GetShaderStageToString(const VkShaderStageFlagBits Stage)
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