#pragma once

#include "Editor/Event.h"
#include "GLFW/glfw3.h"
#include <functional>

namespace South
{

class SWindow
{
public:
    struct SCreateInfo
    {
        bool           bFullscreen = true;
        int            Width       = 0;
        int            Height      = 0;
        std::string    Name;
        SEventCallback EventsCallback;
    };

    explicit SWindow(const SCreateInfo& InInfo);
    ~        SWindow();

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
    SEventCallback m_EventsCallback;

    //~ Events
};

} // namespace South
