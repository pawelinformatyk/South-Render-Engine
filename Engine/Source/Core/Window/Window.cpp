#include "Core/Window/Window.h"

namespace South
{

Window::Window(const CreateInfo& InInfo)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // #TODO : Remove native glfw decorations.
    glfwWindowHint(GLFW_DECORATED, true);

    m_glfwWindow = glfwCreateWindow(InInfo.Width, InInfo.Height, InInfo.Name.c_str(), nullptr, nullptr);

    if(InInfo.bFullscreen)
    {
        glfwMaximizeWindow(m_glfwWindow);
    }

    // #TODO : Remove
    glfwSetWindowPos(m_glfwWindow, 600, 200);

    m_EventsCallback = InInfo.EventsCallback;

    glfwSetWindowUserPointer(m_glfwWindow, &m_EventsCallback);

    glfwSetWindowPosCallback(m_glfwWindow,
                             [](GLFWwindow* InWindow, const int InX, const int InY)
                             {
                                 const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                 Callback(WindowMoveEvent(InX, InY));
                             });

    glfwSetWindowSizeCallback(m_glfwWindow,
                              [](GLFWwindow* InWindow, const int InWidth, const int InHeight)
                              {
                                  const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                  Callback(WindowSizeEvent(InWidth, InHeight));
                              });

    glfwSetWindowCloseCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow)
                               {
                                   const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(WindowCloseEvent());
                               });

    glfwSetWindowFocusCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow, const int InFocused)
                               {
                                   const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(WindowFocusEvent(InFocused));
                               });

    glfwSetWindowIconifyCallback(m_glfwWindow,
                                 [](GLFWwindow* InWindow, int)
                                 {
                                     const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                     Callback(WindowMinimizeEvent());
                                 });

    glfwSetWindowMaximizeCallback(m_glfwWindow,
                                  [](GLFWwindow* InWindow, int)
                                  {
                                      const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                      Callback(WindowMaximizeEvent());
                                  });

    glfwSetKeyCallback(m_glfwWindow,
                       [](GLFWwindow* InWindow, const int InKey, const int InScancode, const int InAction, const int InMods)
                       {
                           const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                           Callback(KeyboardClickEvent(0, 0, 0, InKey, InScancode, InAction, InMods));
                       });

    glfwSetMouseButtonCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow, const int InButton, const int InAction, const int InMods)
                               {
                                   const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(MouseClickEvent(InButton, InAction, InMods));
                               });

    glfwSetCursorPosCallback(m_glfwWindow,
                             [](GLFWwindow* InWindow, const double InX, const double InY)
                             {
                                 const EventCallback& Callback = *static_cast<EventCallback*>(glfwGetWindowUserPointer(InWindow));

                                 Callback(MouseMoveEvent(InX, InY));
                             });
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

// #TODO : Add class TickableObject so app can find these classes and then iterate and tick.
void Window::Tick(const double InDeltaTime)
{
}

void Window::ProcessEvents()
{
    glfwPollEvents();
}

void Window::Minimise()
{
    glfwIconifyWindow(m_glfwWindow);
}

void Window::Maximise()
{
    glfwMaximizeWindow(m_glfwWindow);
}

} // namespace South