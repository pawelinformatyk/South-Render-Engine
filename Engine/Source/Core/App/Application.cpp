#include "sthpch.h"

#include "Core/App/Application.h"

#include "Core/VulkanContext.h"

namespace South
{

    Application::Application() { Instance = this; }

    void Application::Init()
    {
        pWindow = std::make_unique<Window>();
        pWindow->SetCloseWindowCallback([this]() { CloseWindow(); });
        pWindow->Init();

        VulkanContext::Get().Init();

        bRunning = true;
    }

    void Application::Run()
    {
        while (bRunning)
        {
            pWindow->ProcessEvents();

            VulkanContext::Get().Tick();
        }
    }

    void Application::DeInit()
    {
        VulkanContext::Get().DeInit();

        pWindow->SetCloseWindowCallback([]() {});

        pWindow->DeInit();
    }

    Window& Application::GetWindow() const { return *pWindow; }

    void Application::CloseWindow() { bRunning = false; }

    const char* Application::GetName() { return "SouthRenderEngine"; }

    Application& Application::Get() { return *Instance; }

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

} // namespace South