#include "sthpch.h"

#include "Core/Shaders/ShadersLibrary.h"

namespace South
{

    shaderc::Compiler ShadersLibrary::s_Compiler;

    shaderc::CompileOptions ShadersLibrary::s_CompilerOptions;

    ShadersLibrary& ShadersLibrary::Get()
    {
        static ShadersLibrary Instance;
        return Instance;
    }

    void ShadersLibrary::Init()
    {
        s_CompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        s_CompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);

        STH_INFO("ShadersLibrary Initialized.");
    }

    void ShadersLibrary::DeInit()
    {
        for (const auto& [Name, Shader] : m_Shaders) { delete Shader; }

        m_Shaders.clear();

        STH_INFO("ShadersLibrary Deinitialized - shaders cleaned.");
    }

    VulkanShader* ShadersLibrary::AddShader(const std::string& Name, const std::string& PathToCode,
                                            VkShaderStageFlagBits Stages, bool bCompile /*= true*/)
    {
        auto* NewShader = new VulkanShader(PathToCode, Stages, bCompile);
        if (!NewShader) { return nullptr; }

        m_Shaders.emplace(Name, NewShader);

        return NewShader;
    }

    VulkanShader* ShadersLibrary::GetShader(const std::string& Name)
    {
        return m_Shaders.contains(Name) ? m_Shaders[Name] : nullptr;
    }

    const std::unordered_map<std::string, VulkanShader*>& ShadersLibrary::GetShaders() { return m_Shaders; }

    shaderc::Compiler& ShadersLibrary::GetCompiler() { return s_Compiler; }

    shaderc::CompileOptions& ShadersLibrary::GetCompilerOptions() { return s_CompilerOptions; }

} // namespace South