#pragma once

#include <functional>

struct GLFWwindow;

namespace South
{
    using WindowKeyPressedCallback = std::function<void(int Key, int Scancode, int Action, int Mods)>;
    using WindowIconifiedCallback  = std::function<void(bool bIconified)>;
    using WindowMaximisedCallback  = std::function<void()>;


    class Window
    {
    public:
        struct CreateInfo
        {
            bool bFullscreen = true;
            int Width        = 0;
            int Height       = 0;
            std::string Name;
        };

        Window(const CreateInfo& InInfo) : m_Specification(InInfo){};

        // Init glfw window and setup events.
        void Init(const WindowKeyPressedCallback& InOnKeyPressedCallback,
                  const WindowIconifiedCallback& InOnIconifiedCallback,
                  const WindowMaximisedCallback& InOnMaximisedCallbackCallback);
        void DeInit();

        GLFWwindow* GetglfwWindow() const { return m_glfwWindow; }

    private:
        GLFWwindow* m_glfwWindow = nullptr;

        CreateInfo m_Specification;

        //~ Events
    public:
        void ProcessEvents();

        void Minimise();
        void Maximise();

    private:
        struct WindowUserData
        {
            WindowKeyPressedCallback OnWindowKeyPressed;
            WindowIconifiedCallback OnWindowIconified;
            WindowMaximisedCallback OnWindowMaximised;
        } m_WindowUserData;

        //~ Events
    };

} // namespace South
