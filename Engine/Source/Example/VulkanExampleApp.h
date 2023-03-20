#pragma once

namespace South
{

class Editor;
class Window;

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void ProcessEvents();

    float m_FrameTime_Sec = 0.f;

    std::unique_ptr<Window> m_Window = nullptr;

    std::unique_ptr<Editor> m_Editor = nullptr;
};

} // namespace South
