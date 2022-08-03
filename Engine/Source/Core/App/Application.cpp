#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Renderer/Renderer.h"
#include <chrono>

namespace South
{

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::make_unique<Window>(WindowCreateInfo{
            .bFullscreen = true,
            .Width       = 540,
            .Height      = 540,
            .Name        = GetName(),
        });

        m_GUI = std::make_unique<GraphicalInterface>();
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
            m_GUI->BeginFrame();
            {
                Renderer::DrawExampleScene();

                m_GUI->Draw(m_FrameTimeSeconds);
            }
            m_GUI->EndFrame();
            Renderer::EndFrame();

            Renderer::Present();

            std::chrono::time_point FrameEndTime = std::chrono::high_resolution_clock::now();
            m_FrameTimeSeconds                   = std::chrono::duration<float>(FrameEndTime - FrameStartTime).count();
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

        m_GUI->Init();

        m_bRunning = true;
    }

    void Application::DeInit()
    {
        m_bRunning = false;

        m_GUI->DeInit();

        Renderer::DeInit();

        m_Window->DeInit();
    }

    void Application::ProcessEvents()
    {
        if (m_Window) { m_Window->ProcessEvents(); }
    }

    void Application::OnKeyPressed(int Key, int Scancode, int Action, int Mods) {}

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