#pragma once

#include "Core/Shaders/VulkanShader.h"

#include "vulkan/vulkan_core.h"
#include "shaderc/shaderc.hpp"

namespace South
{
    class VulkanShader;

    // Library containing all named compiled shaders in unordererd map.
    // #TODO : Should be part of asset manager? Then change from static methods?
    class ShadersLibrary
    {
      public:
        // Createa instance of class and set static values.
        static void Init();

        static VulkanShader* AddShader(const std::string& name, const std::string& pathToCode,
                                       VkShaderStageFlagBits stages, bool bCompile = true);
        static VulkanShader* GetShader(const std::string& name);

        static const std::unordered_map<std::string, VulkanShader*>& GetShaders();

        static shaderc::Compiler& GetCompiler();
        static shaderc::CompileOptions& GetCompilerOptions();

      private:
        static inline ShadersLibrary* s_ShadersLibrary = nullptr;

        static shaderc::Compiler s_Compiler;
        static shaderc::CompileOptions s_CompilerOptions;

      private:
        ShadersLibrary();
        ~ShadersLibrary();

        std::unordered_map<std::string, VulkanShader*> shaders;
    };

} // namespace South