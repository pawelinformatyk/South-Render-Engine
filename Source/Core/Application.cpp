#include "Core/Application.h"

#include "Core/Tutorial.h"
#include "Core/VulkanContext.h"
#include "Core/Window.h"

namespace South
{
    const char* GetAppName()
    {
        return "SouthRenderEngine";
    }

    Application::Application(int argc, char** argv)
    {
        // #TODO : Creation should be dependant on some static values?
        // And I should not worry about pointers...

        pWindow = AppWindow::Create();
        if (!pWindow)
        {
            return;
        }

        GraphicalContext = new TutorialContext;
        if (!GraphicalContext)
        {
            return;
        }

        GraphicalContext->Init(*pWindow->GetNativeWindow());

        bRunning = true;
    }

    Application::~Application()
    {
        delete pWindow;
        GraphicalContext->DeInit();
        delete GraphicalContext;
    }

    void Application::Run()
    {
        while (bRunning)
        {
            pWindow->Tick();
            GraphicalContext->Tick();
        }
    }

} // namespace South