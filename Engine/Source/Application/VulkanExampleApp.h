#pragma once

#include "Core/Window/Window.h"
#include "Editor/Editor.h"

namespace South
{

// #TODO: Add prefix to classes, it's infuriating
class Application
{
public:
    Application();
    ~Application();

    static Application& Get();

    void Run();

    void OnEvent(const Event& InEvent);

private:
    void Close();
    void Minimize();
    void Maximize();

    void ProcessEvents();

    inline static Application* s_App = nullptr;

    double m_FrameTime_Sec = 0.;

    bool m_bRunning   = true;
    bool m_bMinimized = false;

    std::unique_ptr<Window> m_Window = nullptr;

    std::unique_ptr<Editor> m_Editor = nullptr;
};

} // namespace South
