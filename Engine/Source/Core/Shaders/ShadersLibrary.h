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
        ShadersLibrary(ShadersLibrary const&) = delete;
        void operator=(ShadersLibrary const&) = delete;

        // Createa instance of class and set static values.
        static ShadersLibrary& Get();

        void Init();
        void DeInit();

        VulkanShader* AddShader(const std::string& name, const std::string& pathToCode, VkShaderStageFlagBits stages,
                                bool bCompile = true);
        VulkanShader* GetShader(const std::string& name);

        const std::unordered_map<std::string, VulkanShader*>& GetShaders();

        static shaderc::Compiler& GetCompiler();
        static shaderc::CompileOptions& GetCompilerOptions();

      private:
        ShadersLibrary(){};
        ~ShadersLibrary(){};

        static shaderc::Compiler s_Compiler;
        static shaderc::CompileOptions s_CompilerOptions;

        std::unordered_map<std::string, VulkanShader*> shaders;
    };

} // namespace South