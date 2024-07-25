#pragma once

#include "Editor/Event.h"
#include "GLFW/glfw3.h"
#include <functional>

namespace South
{

class Window
{
public:
    struct CreateInfo
    {
        bool          bFullscreen = true;
        int           Width       = 0;
        int           Height      = 0;
        std::string   Name;
        EventCallback EventsCallback;
    };

    explicit Window(const CreateInfo& InInfo);
    ~Window();

    GLFWwindow* ToGlfw() const;

    void Tick(double InDeltaTime);

private:
    GLFWwindow* m_glfwWindow;

    //~ Events
public:
    void ProcessEvents();

    void Minimise();
    void Maximise();

private:
    EventCallback m_EventsCallback;

    //~ Events
};

} // namespace South
