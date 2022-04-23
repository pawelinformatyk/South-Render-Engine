#include "sthpch.h"

#include "Core/Window/Window.h"
#include "Core/Application.h"

#include <GLFW/glfw3.h>

namespace South
{

    void Window::Init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        glfwWindow = glfwCreateWindow(Width, Height, GetAppName(), nullptr, nullptr);

        glfwSetWindowUserPointer(glfwWindow, &closeWindowFN);

        glfwSetWindowCloseCallback(glfwWindow,
                                   [](GLFWwindow* window)
                                   {
                                       auto& callback = *((CloseWindowCallback*)glfwGetWindowUserPointer(window));
                                       callback();
                                   });
    }

    void Window::DeInit()
    {
        glfwDestroyWindow(glfwWindow);

        glfwTerminate();
    }

    GLFWwindow* Window::GetglfwWindow() const
    {
        return glfwWindow;
    }

    void Window::ProcessEvents()
    {
        glfwPollEvents();
    }

    void Window::SetCloseWindowCallback(const CloseWindowCallback& callback)
    {
        closeWindowFN = callback;
    }

} // namespace South