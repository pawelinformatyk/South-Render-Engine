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
        GLFWwindow* m_glfwWindow = nullptr;

        uint32_t m_Width  = 400;
        uint32_t m_Height = 460;

        // Events
    public:
        using CloseWindowCallback = std::function<void()>;

        void ProcessEvents();
        void SetCloseWindowCallback(const CloseWindowCallback& Callback);

    private:
        CloseWindowCallback m_closeWindowFN;
    };

} // namespace South
