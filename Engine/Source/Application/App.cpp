#include "App.h"

#include "Core/Renderer/Renderer.h"

namespace South
{

SApplication::SApplication()
{
    South::SLogger::Init();

    STH_INFO("Application Start");

    s_App = this;

    m_Window = std::make_unique<SWindow>(SWindow::SCreateInfo {
        // #ifdef _DEBUG
        .bFullscreen = false,
        // #else
        //        .bFullscreen = true,
        // #endif
        .Width  = 1280,
        .Height = 720,
        .Name   = "SouthRenderEngine",
        .EventsCallback =
            [this](const SEvent& InEvent)
        {
            OnEvent(InEvent);
        },
    });

    // #TODO: Where to put it
    SRenderer::Init(SRendererContext::SCreateInfo(*m_Window->ToGlfw()));

    m_Editor = std::make_unique<SEditor>(VkExtent2D(1280, 720), *m_Window->ToGlfw());
}

SApplication::~SApplication()
{
    SRenderer::Deinit();

    STH_INFO("Application End");

    South::SLogger::DeInit();
}

SApplication& SApplication::Get()
{
    return *s_App;
}

void SApplication::Run()
{
    while(m_bRunning)
    {
        m_Window->Tick(m_FrameTime_Sec);

        ProcessEvents();

        std::chrono::time_point FrameStartTime = std::chrono::high_resolution_clock::now();

        // #TODO: Move to to tick or sth
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

        m_Editor->Present();

        m_FrameTime_Sec = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - FrameStartTime).count();
    }
}

void SApplication::ProcessEvents()
{
    m_Window->ProcessEvents();
}

void SApplication::OnEvent(const SEvent& InEvent)
{
    //    STH_INFO(InEvent.ToString());

    // #TODO: Layers: window -> viewport, window -> some gui layer idk
    //    m_Editor->OnEvent(InEvent);
    //
    //    if(const auto* SizeEvent = dynamic_cast<const WindowSizeEvent*>(&InEvent))
    //    {
    //        // #TODO: This should be handled in a Window maybe
    //        RendererContext::Get().GetSwapChain().RecreateSwapChain(SizeEvent->m_Width, SizeEvent->m_Height);
    //    }

    if(InEvent.IsA<SWindowCloseEvent>())
    {
        Close();
    }

    if(InEvent.IsA<SWindowMinimizeEvent>())
    {
        Minimize();
    }

    if(InEvent.IsA<SWindowMaximizeEvent>())
    {
        Maximize();
    }

    if(auto* KeyEvent = dynamic_cast<const SKeyboardClickEvent*>(&InEvent); KeyEvent && KeyEvent->GetKey() == GLFW_KEY_ESCAPE)
    {
        Close();
    }

    m_Editor->OnEvent(InEvent);
}

void SApplication::Close()
{
    m_bRunning = false;
}

void SApplication::Minimize()
{
}

void SApplication::Maximize()
{
}

} // namespace South
