#include "sthpch.h"

#include "Core/Shaders/ShadersLibrary.h"

namespace South
{

    shaderc::Compiler ShadersLibrary::s_Compiler;

    shaderc::CompileOptions ShadersLibrary::s_CompilerOptions;

    ShadersLibrary& ShadersLibrary::Get()
    {
        static ShadersLibrary instance;
        return instance;
    }

    void ShadersLibrary::Init()
    {
        s_CompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        s_CompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    void ShadersLibrary::DeInit()
    {
        for (const auto& [name, shader] : shaders)
        {
            delete shader;
        }

        shaders.clear();
    }

    VulkanShader* ShadersLibrary::AddShader(const std::string& name, const std::string& pathToCode,
                                            VkShaderStageFlagBits stages, bool bCompile /*= true*/)
    {
        auto* newShader = new VulkanShader(pathToCode, stages, bCompile);
        if (!newShader)
        {
            return nullptr;
        }

        shaders.emplace(name, newShader);

        return newShader;
    }

    VulkanShader* ShadersLibrary::GetShader(const std::string& name)
    {
        return shaders[name];
    }

    const std::unordered_map<std::string, VulkanShader*>& ShadersLibrary::GetShaders()
    {
        return shaders;
    }

    shaderc::Compiler& ShadersLibrary::GetCompiler()
    {
        return s_Compiler;
    }

    shaderc::CompileOptions& ShadersLibrary::GetCompilerOptions()
    {
        return s_CompilerOptions;
    }

} // namespace South