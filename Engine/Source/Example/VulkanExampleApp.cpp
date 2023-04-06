#include "sthpch.h"

#include "VulkanExampleApp.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Window/Window.h"
#include "Editor/Editor.h"
#include "GLFW/glfw3.h"

namespace South
{

Application::Application()
{
    s_App = this;

    m_Window = std::make_unique<Window>(Window::CreateInfo {
#ifdef _DEBUG
        .bFullscreen = false,
#else
        .bFullscreen = true,
#endif
        .Width  = 1280,
        .Height = 720,
        .Name   = "South",
        .EventsCallback =
            [this](const Event& InEvent)
        {
            OnEvent(InEvent);
        },
    });

    Renderer::Init(RendererContext::CreateInfo(*m_Window->ToGlfw()));

    m_Editor = std::make_unique<Editor>(*m_Window->ToGlfw());
}

Application::~Application()
{
    Renderer::Deinit();
}

Application& Application::Get()
{
    return *s_App;
}

void Application::Run()
{
    while(m_bRunning)
    {
        if(m_bMinimized)
        {
            continue;
        }

        m_Window->Tick(m_FrameTime_Sec);

        ProcessEvents();

        std::chrono::time_point FrameStartTime = std::chrono::high_resolution_clock::now();

        // Renderer::BeginFrame();
        m_Editor->BeginFrame();
        {
            // #TODO: Move before BeginFrame?
            m_Editor->Tick(m_FrameTime_Sec);

            m_Editor->Render();

            m_Editor->BeginGui();
            m_Editor->RenderGui();
            m_Editor->EndGui();
        }
        m_Editor->EndFrame();
        // Renderer::EndFrame();

        m_Editor->Present();

        m_FrameTime_Sec = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - FrameStartTime).count();
    }
}

void Application::ProcessEvents()
{
    m_Window->ProcessEvents();
}

void Application::OnEvent(const Event& InEvent)
{
    STH_INFO(InEvent.ToString());

    // #TODO: Layers: window -> viewport, window -> some gui layer idk
    m_Editor->OnEvent(InEvent);

    if(InEvent.IsA<WindowCloseEvent>())
    {
        Close();
    }

    if(InEvent.IsA<WindowMinimizeEvent>())
    {
        Minimize();
    }

    if(InEvent.IsA<WindowMaximizeEvent>())
    {
        Maximize();
    }
}

void Application::Close()
{
    m_bRunning = false;
}

void Application::Minimize()
{
    // m_bMinimized = true;
}

void Application::Maximize()
{
    // m_bMinimized = false;
}
} // namespace South
