#pragma once

#include "Core/Shaders/VulkanShader.h"

#include "vulkan/vulkan_core.h"
#include "shaderc/shaderc.hpp"

namespace South
{

    // Library containing all named compiled shaders in unordererd map.
    // #TODO : Should be part of asset manager?
    // Part of rendered but idk?
    class ShadersLibrary
    {
    public:
        static void Init();
        static void DeInit();

        static VulkanShader* AddShader(const std::string& Name, const std::string& PathToCode,
                                       VkShaderStageFlagBits Stages, bool bCompile = true);
        static VulkanShader* GetShader(const std::string& Name);

        static const std::unordered_map<std::string, VulkanShader*>& GetShaders();

        static shaderc::Compiler& GetCompiler();
        static shaderc::CompileOptions& GetCompilerOptions();

    private:
        static shaderc::Compiler s_Compiler;
        static shaderc::CompileOptions s_CompilerOptions;

        static std::unordered_map<std::string, VulkanShader*> s_Shaders;
    };

} // namespace South