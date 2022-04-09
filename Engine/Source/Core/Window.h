#pragma once

#include "vulkan/vk_platform.h"

struct GLFWwindow;

namespace South
{

    // Target of graphics rendering and inputs.
    // Events need to be picked up by this window
    // #TODO : I think there should be swap chain here.
    class Window
    {
      public:
        void Init();
        void DeInit();

        void ProcessEvents();

        GLFWwindow* GetglfwWindow() const;

      private:
        GLFWwindow* glfwWindow = nullptr;

        uint32_t Width  = 1600;
        uint32_t Height = 960;
    };

} // namespace South
