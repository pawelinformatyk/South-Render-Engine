#include "sthpch.h"

#include "Core/ShadersLibrary.h"

namespace South
{

        {
        if (!s_ShadersLibrary)
        {
            s_ShadersLibrary = new ShadersLibrary;

        {
        }

            if (!newShader)
            {
                return nullptr;
            }


            return newShader;
        }

        {
            return shaders[name];
        }


} // namespace South