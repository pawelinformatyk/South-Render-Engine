#pragma once

#include "Core/Shaders/Shader.h"

#include "shaderc/shaderc.hpp"
#include "vulkan/vulkan_core.h"

namespace South
{

// Library containing all named compiled shaders in unordererd map.
// #TODO : Should be part of asset manager?
// Part of rendered but idk?
class ShadersLibrary
{
public:
    static void Init();
    static void Deinit();


    static Shader* AddShader(VkDevice Device, const std::string& Name, VkShaderStageFlagBits Stage, bool bCompile = true);
    static Shader* AddShader(VkDevice              Device,
                             const std::string&    Name,
                             const std::string&    PathToCode,
                             VkShaderStageFlagBits Stage,
                             bool                  bCompile = true);
    static Shader* GetShader(const std::string& Name);

    static const std::unordered_map<std::string, Shader*>& GetShaders();

    static shaderc::Compiler&       GetCompiler();
    static shaderc::CompileOptions& GetCompilerOptions();

    static const char* GetShaderStageToString(VkShaderStageFlagBits Stage);

    static inline std::string s_ShadersDirectory = "Shaders/";

private:
    static inline shaderc::Compiler       s_Compiler;
    static inline shaderc::CompileOptions s_CompilerOptions;

    static inline std::unordered_map<std::string, Shader*> s_Shaders;
};

} // namespace South