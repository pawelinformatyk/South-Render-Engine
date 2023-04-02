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
    m_Window = std::make_unique<Window>(Window::CreateInfo {
        .bFullscreen = false,
        .Width       = 1280,
        .Height      = 720,
        .Name        = "South",
    });

    m_Editor = std::make_unique<Editor>(*m_Window->ToGlfw());
}

Application::~Application() = default;

void Application::Run()
{
    while(!glfwWindowShouldClose(m_Window->ToGlfw()))
    {
        m_Window->Tick(m_FrameTime_Sec);

        ProcessEvents();

        std::chrono::time_point FrameStartTime = std::chrono::high_resolution_clock::now();

        // Renderer::BeginFrame();
        m_Editor->BeginFrame();
        {
            // #TODO: Move before BeginFrame?
            m_Editor->Tick();

            m_Editor->Render();

            m_Editor->BeginGUI();
            m_Editor->RenderGUI();
            m_Editor->EndGUI();
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

} // namespace South
