#include "sthpch.h"

#include "Core/Window/Window.h"
#include <GLFW/glfw3.h>

namespace South
{

Window::Window(const CreateInfo& InInfo) : m_Specification(InInfo)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    const auto& [bFullscreen, Width, Height, Name] = InInfo;

    m_glfwWindow = glfwCreateWindow(Width, Height, Name.c_str(), nullptr, nullptr);

    if(bFullscreen)
    {
        glfwMaximizeWindow(m_glfwWindow);
    }

    // auto&           glfwWindow = *Window.m_glfwWindow;
    // WindowUserData& UserData   = Window.m_WindowUserData;
    //
    // Window.m_WindowUserData.OnWindowKeyPressed = InOnKeyPressedCallback;
    // Window.m_WindowUserData.OnMousePosChange   = InOnMousePosChange;
    // m_WindowUserData.OnWindowIconified  = InOnIconifiedCallback;
    // m_WindowUserData.OnWindowMaximised  = InOnMaximisedCallback;

    // glfwSetWindowUserPointer(&glfwWindow, &UserData);
    //
    // glfwSetKeyCallback(&glfwWindow,
    //                    [](GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
    //                    {
    //                        const WindowUserData& UserData = *static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));
    //
    //                        if(UserData.OnWindowKeyPressed)
    //                        {
    //                            UserData.OnWindowKeyPressed(Key, Scancode, Action, Mods);
    //                        }
    //                    });
    //
    // glfwSetWindowIconifyCallback(m_glfwWindow,
    //                              [](GLFWwindow* Window, int Iconified)
    //                              {
    //                                  WindowUserData* UserData = static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));
    //
    //                                  if(UserData && UserData->OnWindowIconified)
    //                                  {
    //                                      UserData->OnWindowIconified(Iconified);
    //                                  }
    //                              });
    //
    // glfwSetWindowMaximizeCallback(m_glfwWindow,
    //                               [](GLFWwindow* Window, int Maximized)
    //                               {
    //                                   WindowUserData* UserData = static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));
    //
    //                                   if(UserData && UserData->OnWindowMaximised)
    //                                   {
    //                                       UserData->OnWindowMaximised();
    //                                   }
    //                               });
    //
    // glfwSetCursorPosCallback(&glfwWindow,
    //                          [](GLFWwindow* Window, double X, double Y)
    //                          {
    //                              const WindowUserData& UserData = *static_cast<WindowUserData*>(glfwGetWindowUserPointer(Window));
    //
    //                              if(UserData.OnMousePosChange)
    //                              {
    //                                  UserData.OnMousePosChange(X, Y);
    //                              }
    //                          });
}

Window::~Window()
{
    glfwDestroyWindow(m_glfwWindow);

    glfwTerminate();
}

GLFWwindow* Window::ToGlfw() const
{
    return m_glfwWindow;
}

void Window::ProcessEvents()
{
    glfwPollEvents();
}

// void Window::Minimise()
// {
//     glfwIconifyWindow(m_glfwWindow);
// }
//
// void Window::Maximise()
// {
//     glfwMaximizeWindow(m_glfwWindow);
// }

} // namespace South