#include "sthpch.h"

#include "Core/ShadersLibrary.h"

namespace South
{

    namespace ShadersLibrary
    {

        void CompileAllShaders()
        {
            system("Resources\\Shaders\\CompileShaders.bat");
        }

        VulkanShader* AddShader(const std::string& name, const std::string& pathToSPIRV, ShaderType inType)
        {
            auto* newShader = new VulkanShader(name, pathToSPIRV, inType);
            if (!newShader)
            {
                return nullptr;
            }

            shaders.emplace(name, newShader);

            return newShader;
        }

        VulkanShader* GetShader(const std::string& name)
        {
            return shaders[name];
        }

    } // namespace ShadersLibrary

} // namespace South