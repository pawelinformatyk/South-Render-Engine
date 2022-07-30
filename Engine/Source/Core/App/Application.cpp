#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Renderer/Renderer.h"

namespace South
{

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

    Application::Application()
    {
        if (s_Instance) { return; }

        s_Instance = this;
        m_Window   = std::make_unique<Window>();
        m_GUI      = std::make_unique<GraphicalInterface>();
    }

    Application::~Application() { s_Instance = nullptr; }

    void Application::Run()
    {
        Init();

        while (m_bRunning)
        {
            ProcessEvents();

            Renderer::BeginFrame();
            m_GUI->BeginFrame();
            {
                Renderer::DrawExampleScene();

                m_GUI->Draw();
            }
            m_GUI->EndFrame();
            Renderer::EndFrame();

            Renderer::Present();
        }

        DeInit();
    }

    void Application::Init()
    {
        m_Window->SetIconifyWindowCallback([this](bool bMinimised) { MinimiseApplication(bMinimised); });
        m_Window->Init();

        Renderer::Init();

        m_GUI->Init();

        m_bRunning = true;
    }

    void Application::DeInit()
    {
        m_bRunning = false;

        m_GUI->DeInit();

        Renderer::DeInit();

        m_Window->SetIconifyWindowCallback([](bool bMinimised) {});
        m_Window->DeInit();
    }

    void Application::ProcessEvents() { m_Window->ProcessEvents(); }

    void Application::CloseApplication() { m_bRunning = false; }

    void Application::MaximiseApplication() {}

    void Application::MinimiseApplication(bool bMinimized)
    {
        // #TODO : Reduce FPS instead of this flag.
        // if (m_WindowMinimized == bMinimized) { return; }

        // m_WindowMinimized = bMinimized;

        // if (m_Window) { m_Window->Iconify(bMinimized); }
    }

} // namespace South