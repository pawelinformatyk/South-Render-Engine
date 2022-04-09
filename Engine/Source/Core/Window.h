#pragma once

#include <functional>

struct GLFWwindow;

namespace South
{

    // Target of graphics rendering and inputs.
    // Events need to be picked up by this window
    // #TODO : I think there should be swap chain here.
    class Window
    {
        using CloseWindowCallback = std::function<void()>;

      public:
        void Init();
        void DeInit();

        void ProcessEvents();

        GLFWwindow* GetglfwWindow() const;

        void SetCloseWindowCallback(const CloseWindowCallback& callback);

      private:
        GLFWwindow* glfwWindow = nullptr;

        uint32_t Width  = 1600;
        uint32_t Height = 960;

        CloseWindowCallback closeWindowFN;
    };

} // namespace South
