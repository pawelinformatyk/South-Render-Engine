#include "sthpch.h"

#include "Core/Shaders/ShadersLibrary.h"

namespace South
{

    shaderc::Compiler ShadersLibrary::s_Compiler;

    shaderc::CompileOptions ShadersLibrary::s_CompilerOptions;

    void ShadersLibrary::Init()
    {
        if (!s_ShadersLibrary)
        {
            s_ShadersLibrary = new ShadersLibrary;

            // #TODO : Should be somewhere else.
            s_CompilerOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
            s_CompilerOptions.SetOptimizationLevel(shaderc_optimization_level_performance);
        }
    }

    VulkanShader* ShadersLibrary::AddShader(const std::string& name, const std::string& pathToCode,
                                            VkShaderStageFlagBits stages, bool bCompile /*= true*/)
    {
        if (!s_ShadersLibrary)
        {
            return nullptr;
        }

        auto* newShader = new VulkanShader(pathToCode, stages, bCompile);
        if (!newShader)
        {
            return nullptr;
        }

        s_ShadersLibrary->shaders.emplace(name, newShader);

        return newShader;
    }

    VulkanShader* ShadersLibrary::GetShader(const std::string& name)
    {
        return s_ShadersLibrary->shaders[name];
    }

    const std::unordered_map<std::string, VulkanShader*>& ShadersLibrary::GetShaders()
    {
        return s_ShadersLibrary->shaders;
    }

    shaderc::Compiler& ShadersLibrary::GetCompiler()
    {
        return s_Compiler;
    }

    shaderc::CompileOptions& ShadersLibrary::GetCompilerOptions()
    {
        return s_CompilerOptions;
    }

    ShadersLibrary::ShadersLibrary()
    {
    }

    ShadersLibrary::~ShadersLibrary()
    {
    }

} // namespace South