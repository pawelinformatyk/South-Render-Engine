#pragma once

#include <functional>

struct GLFWwindow;

namespace South
{

    class Window
    {
    public:
        using KeyPressedCallback = std::function<void(int Key, int Scancode, int Action, int Mods)>;
        using IconifiedCallback  = std::function<void(bool bIconified)>;
        using MaximisedCallback  = std::function<void()>;

        struct CreateInfo
        {
            bool bFullscreen = true;
            int Width        = 0;
            int Height       = 0;
            std::string Name;
        };

        Window(const CreateInfo& InInfo) : m_Specification(InInfo){};

        // Init glfw window and setup events.
        void Init(const KeyPressedCallback& InOnKeyPressedCallback,
                  const IconifiedCallback& InOnIconifiedCallback,
                  const MaximisedCallback& InOnMaximisedCallbackCallback);
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
            KeyPressedCallback OnWindowKeyPressed;
            IconifiedCallback OnWindowIconified;
            MaximisedCallback OnWindowMaximised;
        } m_WindowUserData;

        //~ Events
    };

} // namespace South
