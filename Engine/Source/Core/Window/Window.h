#pragma once

#include "Editor/Event.h"

#include <functional>

struct GLFWwindow;

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

    Window(const CreateInfo& InInfo);
    ~Window();

    GLFWwindow* ToGlfw() const;

    void Tick(double InDeltaTime);

private:
    GLFWwindow* m_glfwWindow = nullptr;

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
