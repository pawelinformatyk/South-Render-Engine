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
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        auto* Monitor              = glfwGetPrimaryMonitor();
        const GLFWvidmode* VidMode = glfwGetVideoMode(Monitor);
        m_glfwWindow = glfwCreateWindow(VidMode->width, VidMode->height, Application::GetName(), nullptr, nullptr);

        glfwMaximizeWindow(m_glfwWindow);

        // Events
        {
            glfwSetWindowUserPointer(m_glfwWindow, &m_WindowUserData);

            // #TODO : Should app dispatch events or window.
            // What is prettier? I Think app.
            // Its the same in the end.
            // And why not just functions. I GUESS JUST TEST AND SEE LMAO

            glfwSetWindowIconifyCallback(m_glfwWindow,
                                         [](GLFWwindow* Window, int Iconified)
                                         {
                                             WindowUserData* UserData =
                                                 static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));

                                             if (UserData && UserData->IconifyWindowFunction)
                                             {
                                                 UserData->IconifyWindowFunction(Iconified);
                                             }
                                         });
        }
    }

    void Window::DeInit()
    {
        glfwDestroyWindow(m_glfwWindow);

        glfwTerminate();
    }

    void Window::ProcessEvents() { glfwPollEvents(); }

    void Window::Iconify(bool bMinimised) { glfwIconifyWindow(m_glfwWindow); }

} // namespace South