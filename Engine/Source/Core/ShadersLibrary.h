#pragma once

#include "Core/VulkanShader.h"

#include "vulkan/vulkan_core.h"

namespace South
{
    class VulkanShader;

    namespace
    {
        std::unordered_map<std::string, VulkanShader*> shaders;
    } // namespace

    namespace ShadersLibrary
    {
        void CompileAllShaders();

        // #TODO : Instaed of pathToSPIRV give path to code and then compile.
        VulkanShader* AddShader(const std::string& name, const std::string& pathToSPIRV, ShaderType inType);
        VulkanShader* GetShader(const std::string& name);

    } // namespace ShadersLibrary

} // namespace South