#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace South
{
    AppWindow::AppWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        pWindow = glfwCreateWindow(Width, Height, "SouthRenderEngine", nullptr, nullptr);
    }

    AppWindow::~AppWindow()
    {
        glfwDestroyWindow(pWindow);

        glfwTerminate();
    }

    void AppWindow::Tick()
    {
        glfwPollEvents();
    }

    AppWindow* AppWindow::Create()
    {
        return new AppWindow;
    }
} // namespace South