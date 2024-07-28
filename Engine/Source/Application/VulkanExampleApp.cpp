#include "VulkanExampleApp.h"

#include "Core/Renderer/Renderer.h"

namespace South
{

Application::Application()
{
    South::Logger::Init();

    STH_INFO("Application Start");

    s_App = this;

    m_Window = std::make_unique<Window>(Window::CreateInfo {
        // #ifdef _DEBUG
        .bFullscreen = false,
        // #else
        //        .bFullscreen = true,
        // #endif
        .Width  = 1280,
        .Height = 720,
        .Name   = "SouthRenderEngine",
        .EventsCallback =
            [this](const Event& InEvent)
        {
            OnEvent(InEvent);
        },
    });

    Renderer::Init(RendererContext::CreateInfo(*m_Window->ToGlfw()));

    m_Editor = std::make_unique<Editor>(VkExtent2D(1280, 720), *m_Window->ToGlfw());
}

Application::~Application()
{
    Renderer::Deinit();

    STH_INFO("Application End");

    South::Logger::DeInit();
}

Application& Application::Get()
{
    return *s_App;
}

void Application::Run()
{
    while(m_bRunning)
    {
        m_Window->Tick(m_FrameTime_Sec);

        ProcessEvents();

        std::chrono::time_point FrameStartTime = std::chrono::high_resolution_clock::now();

        Renderer::BeginFrame();
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
        Renderer::EndFrame();

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
    //    m_Editor->OnEvent(InEvent);
    //
    //    if(const auto* SizeEvent = dynamic_cast<const WindowSizeEvent*>(&InEvent))
    //    {
    //        // #TODO: This should be handled in a Window maybe
    //        RendererContext::Get().GetSwapChain().RecreateSwapChain(SizeEvent->m_Width, SizeEvent->m_Height);
    //    }


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

    if(auto* KeyEvent = dynamic_cast<const KeyboardClickEvent*>(&InEvent); KeyEvent && KeyEvent->GetKey() == GLFW_KEY_ESCAPE)
    {
        Close();
    }

    m_Editor->OnEvent(InEvent);
}

void Application::Close()
{
    m_bRunning = false;
}

void Application::Minimize()
{
}

void Application::Maximize()
{
}
} // namespace South
