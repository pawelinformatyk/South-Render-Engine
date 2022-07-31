#include "sthpch.h"

#include "Core/Window/Window.h"
#include <GLFW/glfw3.h>

namespace South
{

    void Window::Init(const WindowKeyPressedCallback& InOnKeyPressedCallback,
                      const WindowIconifiedCallback& InOnIconifiedCallback,
                      const WindowMaximisedCallback& InOnMaximisedCallback)
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        m_glfwWindow = glfwCreateWindow(m_Specification.Width,
                                        m_Specification.Height,
                                        m_Specification.Name.c_str(),
                                        nullptr,
                                        nullptr);

        if (m_Specification.bFullscreen) { glfwMaximizeWindow(m_glfwWindow); }

        // Events
        {
            m_WindowUserData.OnWindowKeyPressed = InOnKeyPressedCallback;
            m_WindowUserData.OnWindowIconified  = InOnIconifiedCallback;
            m_WindowUserData.OnWindowMaximised  = InOnMaximisedCallback;

            glfwSetWindowUserPointer(m_glfwWindow, &m_WindowUserData);

            glfwSetKeyCallback(m_glfwWindow,
                               [](GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
                               {
                                   WindowUserData* UserData =
                                       static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));

                                   if (UserData && UserData->OnWindowKeyPressed)
                                   {
                                       UserData->OnWindowKeyPressed(Key, Scancode, Action, Mods);
                                   }
                               });

            glfwSetWindowIconifyCallback(m_glfwWindow,
                                         [](GLFWwindow* Window, int Iconified)
                                         {
                                             WindowUserData* UserData =
                                                 static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));

                                             if (UserData && UserData->OnWindowIconified)
                                             {
                                                 UserData->OnWindowIconified(Iconified);
                                             }
                                         });

            glfwSetWindowMaximizeCallback(m_glfwWindow,
                                          [](GLFWwindow* Window, int Maximized)
                                          {
                                              WindowUserData* UserData =
                                                  static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));

                                              if (UserData && UserData->OnWindowMaximised)
                                              {
                                                  UserData->OnWindowMaximised();
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

    void Window::Minimise()
    {
        // #TODO : Edit m_Specification.
        glfwIconifyWindow(m_glfwWindow);
    }

    void Window::Maximise()
    {
        // #TODO : Edit m_Specification.
        glfwMaximizeWindow(m_glfwWindow);
    }

} // namespace South