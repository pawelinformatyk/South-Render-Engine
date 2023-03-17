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
    using MousePosCallback   = std::function<void(double, double)>;

    struct CreateInfo
    {
        bool        bFullscreen = true;
        int         Width       = 0;
        int         Height      = 0;
        std::string Name;
    };

    explicit Window(const CreateInfo& InInfo);

    static void Init(Window&                   Window,
                     const KeyPressedCallback& InOnKeyPressedCallback,
                     const IconifiedCallback&  InOnIconifiedCallback,
                     const MaximisedCallback&  InOnMaximisedCallbackCallback,
                     const MousePosCallback&   InOnMousePosChange);
    static void DeInit(Window& Window);

    GLFWwindow* ToGlfw() const
    {
        return m_glfwWindow;
    }

private:
    GLFWwindow* m_glfwWindow = nullptr;

    CreateInfo m_Specification;

    //~ Events
    // #TODO : public handle to delegate.
public:
    void ProcessEvents();

    void Minimise();
    void Maximise();

private:
    struct WindowUserData
    {
        KeyPressedCallback OnWindowKeyPressed;
        IconifiedCallback  OnWindowIconified;
        MaximisedCallback  OnWindowMaximised;
        MousePosCallback   OnMousePosChange;
    } m_WindowUserData;

    //~ Events
};

} // namespace South
