#pragma once

#include "Core/App/GraphicalInterface.h"
#include "Core/Window/Window.h"

namespace South
{

class Application
{
public:
    Application();
    ~Application();

    void Run();

    Window& GetWindow() const;

    static const char*  GetName();
    static Application& Get();
    static void         Kaboom();

private:
    static inline Application* s_Instance = nullptr;

    void Init();
    void DeInit();

    std::unique_ptr<GraphicalInterface> m_Gui    = nullptr;
    std::unique_ptr<Window>             m_Window = nullptr;

    bool m_bRunning = false;

    float m_FrameTime_Sec = 0.f;

    //~ Events
public:
    void Minimise();
    void Maximise();
    void Close();

private:
    void ProcessEvents();

    void OnKeyPressed(int InKey, int InScancode, int InAction, int InMods);
    void OnIconifiedWindow(int InbIconified);
    void OnMaximiseWindow();
    //~ Events.
};

// Renderer -> move vulkan functions to this. I mean like instance etc.

// I want to have multiple viewports. How do I do it. Do I need layers or just
// two imgui windows with rendered scenes with different camera.
// VkPipelineViewportStateCreateInfo - > viewport count?

// Multiple viewports -> multiple textures rendered and shown from different VIEW matrix.
// I thjink I need to implement vulcan code so i can then decide how to do it.
// What is framebuffer exactly?

} // namespace South