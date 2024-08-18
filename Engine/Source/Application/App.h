#pragma once

#include "Core/Window/Window.h"
#include "Editor/Editor.h"

namespace South
{

class SApplication
{
public:
     SApplication();
    ~SApplication();

    static SApplication& Get();

    void Run();

    void OnEvent(const SEvent& InEvent);

private:
    void Close();
    void Minimize();
    void Maximize();

    void ProcessEvents();

    inline static SApplication* s_App = nullptr;

    double m_FrameTime_Sec = 0.;

    bool m_bRunning   = true;
    bool m_bMinimized = false;

    std::unique_ptr<SWindow> m_Window = nullptr;

    std::unique_ptr<SEditor> m_Editor = nullptr;
};

} // namespace South
