#include "sthpch.h"

#include "Core/Window.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace South
{

    void Window::Init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindow = glfwCreateWindow(Width, Height, GetAppName(), nullptr, nullptr);
    }

    void Window::DeInit()
    {
        glfwDestroyWindow(glfwWindow);

        glfwTerminate();
    }

    void Window::ProcessEvents()
    {
        glfwPollEvents();
    }

    GLFWwindow* Window::GetglfwWindow() const
    {
        return glfwWindow;
    }

} // namespace South