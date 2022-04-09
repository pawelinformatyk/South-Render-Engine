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
        Instance = this;

        pWindow = std::make_unique<Window>();
        pWindow->SetCloseWindowCallback(
            [this]()
            {
                CloseWindow();
            });
        pWindow->Init();

        GraphicalContext = new VulkanContext;
        if (!GraphicalContext)
        {
            return;
        }

        GraphicalContext->Init();

        bRunning = true;
    }

    void Application::Run()
    {
        while (bRunning)
        {
            pWindow->ProcessEvents();
            GraphicalContext->Tick();
        }
    }

    void Application::DeInit()
    {
        GraphicalContext->DeInit();
        pWindow->SetCloseWindowCallback(
            []()
            {
            });

        pWindow->DeInit();
        delete GraphicalContext;
    }

    Window& Application::GetWindow() const
    {
        return *pWindow;
    }

    Application& Application::Get()
    {
        return *Instance;
    }

    void Application::CloseWindow()
    {
        bRunning = false;
    }

} // namespace South