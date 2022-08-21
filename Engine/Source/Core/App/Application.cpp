#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Renderer/Renderer.h"
#include <chrono>

namespace South
{
    const char* Application::GetName() { return "SouthRenderEngine"; }

    Application& Application::Get() { return *s_Instance; }

    Window& Application::GetWindow() const { return *m_Window; }

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::make_unique<Window>(Window::CreateInfo{
            .bFullscreen = true,
            .Width       = 540,
            .Height      = 540,
            .Name        = GetName(),
        });

        m_Gui = std::make_unique<GraphicalInterface>();
    }

    Application::~Application() { s_Instance = nullptr; }

    void Application::Run()
    {
        Init();

        while (m_bRunning)
        {
            ProcessEvents();

            std::chrono::time_point FrameStartTime = std::chrono::high_resolution_clock::now();

            Renderer::BeginFrame();
            m_Gui->BeginFrame();
            {
                Renderer::DrawExampleScene();

                m_Gui->Draw(m_FrameTime_Sec);
            }
            m_Gui->EndFrame();
            Renderer::EndFrame();

            Renderer::Present();

            std::chrono::time_point FrameEndTime = std::chrono::high_resolution_clock::now();
            m_FrameTime_Sec                   = std::chrono::duration<float>(FrameEndTime - FrameStartTime).count();
        }

        DeInit();
    }

    void Application::Init()
    {
        m_Window->Init([this](int Key, int Scancode, int Action, int Mods)
                       { OnKeyPressed(Key, Scancode, Action, Mods); },
                       [this](int bIconified) { OnIconifiedWindow(bIconified); },
                       [this]() { OnMaximiseWindow(); });

        Renderer::Init();

        m_Gui->Init();

        m_bRunning = true;
    }

    void Application::DeInit()
    {
        m_bRunning = false;

        m_Gui->DeInit();

        Renderer::Deinit();

        m_Window->DeInit();
    }

    void Application::ProcessEvents()
    {
        if (m_Window) { m_Window->ProcessEvents(); }
    }

    void Application::OnKeyPressed(int InKey, int InScancode, int InAction, int InMods) {}

    void Application::Minimise()
    {
        if (m_Window) { m_Window->Minimise(); }
    }

    void Application::Maximise()
    {
        if (m_Window) { m_Window->Maximise(); }
    }

    void Application::OnIconifiedWindow(int bIconified) {}

    void Application::OnMaximiseWindow() {}

    void Application::Close() { m_bRunning = false; }


} // namespace South