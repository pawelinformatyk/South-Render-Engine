#include "sthpch.h"

#include "Core/Shaders/ShadersLibrary.h"

namespace South
{

shaderc::Compiler                        ShadersLibrary::s_Compiler;
shaderc::CompileOptions                  ShadersLibrary::s_CompilerOptions;
std::unordered_map<std::string, Shader*> ShadersLibrary::s_Shaders;

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

Shader* ShadersLibrary::AddShader(VkDevice              Device,
                                  const std::string&    Name,
                                  const std::string&    PathToCode,
                                  VkShaderStageFlagBits Stages,
                                  bool                  bCompile /*= true*/)
{
    auto* NewShader = new Shader(Device, PathToCode, Stages, bCompile);
    if(!NewShader)
    {
        return nullptr;
    }

    s_Shaders.emplace(Name, NewShader);

    STH_INFO("ShadersLibrary : {:s}-shader {:s} added.", GetShaderStageToString(Stages), Name);

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

const char* ShadersLibrary::GetShaderStageToString(VkShaderStageFlagBits Stage)
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