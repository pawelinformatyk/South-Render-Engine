#pragma once

#include <functional>

struct GLFWwindow;

namespace South
{

    // Target of graphics rendering and inputs.
    // Events need to be picked up by this window
    class Window
    {

      public:
        virtual void Init();
        virtual void DeInit();

        GLFWwindow* GetglfwWindow() const;

      protected:
        GLFWwindow* glfwWindow = nullptr;

        uint32_t Width  = 400;
        uint32_t Height = 460;

        // Events
      public:
        using CloseWindowCallback = std::function<void()>;

        void ProcessEvents();
        void SetCloseWindowCallback(const CloseWindowCallback& callback);

      private:
        CloseWindowCallback closeWindowFN;
    };

} // namespace South
