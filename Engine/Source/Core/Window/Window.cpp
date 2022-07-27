#include "sthpch.h"

#include "Core/Window/Window.h"
#include "Core/App/Application.h"

#include <GLFW/glfw3.h>

namespace South
{

    void Window::Init()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_glfwWindow = glfwCreateWindow(m_Width, m_Height, Application::GetName(), nullptr, nullptr);

        glfwSetWindowUserPointer(m_glfwWindow, &m_closeWindowFN);

        glfwSetWindowCloseCallback(m_glfwWindow,
                                   [](GLFWwindow* window)
                                   {
                                       auto& callback = *((CloseWindowCallback*)glfwGetWindowUserPointer(window));
                                       callback();
                                   });
    }

    void Window::DeInit()
    {
        glfwDestroyWindow(m_glfwWindow);

        glfwTerminate();
    }

    GLFWwindow* Window::GetglfwWindow() const { return m_glfwWindow; }

    void Window::ProcessEvents() { glfwPollEvents(); }

    void Window::SetCloseWindowCallback(const CloseWindowCallback& Callback) { m_closeWindowFN = Callback; }

} // namespace South