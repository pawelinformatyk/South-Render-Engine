#include "sthpch.h"

#include "Core/Application.h"

#include "Core/VulkanContext.h"

namespace South
{

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

    const char* Application::GetName()
    {
        return "SouthRenderEngine";
    }

    Application& Application::Get()
    {
        static Application Instance;
        return Instance;
    }

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

} // namespace South