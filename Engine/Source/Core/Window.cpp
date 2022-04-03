#include "sthpch.h"

#include "Core/Window.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace South
{
    AppWindow::AppWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        pWindow = glfwCreateWindow(Width, Height, GetAppName(), nullptr, nullptr);
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

    GLFWwindow* AppWindow::GetNativeWindow() const
    {
        return pWindow;
    }

    AppWindow* AppWindow::Create()
    {
        return new AppWindow;
    }
} // namespace South