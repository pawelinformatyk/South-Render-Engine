#pragma once

#include <functional>

struct GLFWwindow;

namespace South
{

class Window
{
public:
    struct CreateInfo
    {
        bool        bFullscreen = true;
        int         Width       = 0;
        int         Height      = 0;
        std::string Name;
    };

    Window(const CreateInfo& InInfo);
    ~Window();

    GLFWwindow* ToGlfw() const;

private:
    GLFWwindow* m_glfwWindow = nullptr;

    CreateInfo m_Specification;

    //~ Events
    // #TODO : public handle to delegate.
public:
    void ProcessEvents();

    // void Minimise();
    // void Maximise();

private:
    // using KeyPressedCallback = std::function<void(int Key, int Scancode, int Action, int Mods)>;
    // using IconifiedCallback  = std::function<void(bool bIconified)>;
    // using MaximisedCallback  = std::function<void()>;
    // using MousePosCallback   = std::function<void(double, double)>;
    //
    // struct WindowUserData
    // {
    //     KeyPressedCallback OnWindowKeyPressed;
    //     IconifiedCallback  OnWindowIconified;
    //     MaximisedCallback  OnWindowMaximised;
    //     MousePosCallback   OnMousePosChange;
    // } m_WindowUserData;

    //~ Events
};

} // namespace South
