#pragma once

#include "Core/Shaders/VulkanShader.h"

#include "vulkan/vulkan_core.h"
#include "shaderc/shaderc.hpp"

namespace South
{
    class VulkanShader;

    // Library containing all named compiled shaders in unordererd map.
    // #TODO : Should be part of asset manager?
    class ShadersLibrary
    {
      public:
        static ShadersLibrary& Get();

        ShadersLibrary(ShadersLibrary const&) = delete;
        void operator=(ShadersLibrary const&) = delete;

        void Init();
        void DeInit();

        VulkanShader* AddShader(const std::string& Name, const std::string& PathToCode, VkShaderStageFlagBits Stages,
                                bool bCompile = true);
        VulkanShader* GetShader(const std::string& Name);

        const std::unordered_map<std::string, VulkanShader*>& GetShaders();

        static shaderc::Compiler& GetCompiler();
        static shaderc::CompileOptions& GetCompilerOptions();

      private:
        ShadersLibrary(){};
        ~ShadersLibrary(){};

        static shaderc::Compiler s_Compiler;
        static shaderc::CompileOptions s_CompilerOptions;

        std::unordered_map<std::string, VulkanShader*> Shaders;
    };

} // namespace South