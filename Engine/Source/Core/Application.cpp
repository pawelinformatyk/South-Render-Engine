#include "sthpch.h"

#include "Core/Application.h"

#include "Core/VulkanContext.h"

namespace South
{
    const char* GetAppName()
    {
        return "SouthRenderEngine";
    }

    void Application::Init()
    {
        pWindow = std::make_unique<Window>();
        pWindow->SetCloseWindowCallback(
            [this]()
            {
                CloseWindow();
            });
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

        pWindow->SetCloseWindowCallback(
            []()
            {
            });

        pWindow->DeInit();
    }

    Window& Application::GetWindow() const
    {
        return *pWindow;
    }

    void Application::CloseWindow()
    {
        bRunning = false;
    }

    Application& Application::Get()
    {
        static Application instance;
        return instance;
    }

    void Application::Kaboom()
    {
        *(int*)0 = 0;
    }

} // namespace South