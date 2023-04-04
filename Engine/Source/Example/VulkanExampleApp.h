#pragma once

namespace South
{

class Event;
class Editor;
class Window;

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

    bool m_bRunning = true;

    std::unique_ptr<Window> m_Window = nullptr;

    std::unique_ptr<Editor> m_Editor = nullptr;
};

} // namespace South
