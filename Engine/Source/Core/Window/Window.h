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

        GLFWwindow* GetglfwWindow() const { return m_glfwWindow; }

    private:
        GLFWwindow* m_glfwWindow = nullptr;

        //~ Events
    public:
        using CloseWindowCallback = std::function<void(bool bMinimised)>;

        void ProcessEvents();

        void Iconify(bool bMinimised);

        void SetIconifyWindowCallback(const CloseWindowCallback& Callback)
        {
            m_WindowUserData.IconifyWindowFunction = Callback;
        }

    private:
        struct WindowUserData
        {
            CloseWindowCallback IconifyWindowFunction;
        } m_WindowUserData;

        //~ Events
    };

} // namespace South
