#include "Core/Window/Window.h"

namespace South
{

SWindow::SWindow(const SCreateInfo& InInfo)
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
                                 const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                 Callback(SWindowMoveEvent(InX, InY));
                             });

    glfwSetWindowSizeCallback(m_glfwWindow,
                              [](GLFWwindow* InWindow, const int InWidth, const int InHeight)
                              {
                                  const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                  Callback(SWindowSizeEvent(InWidth, InHeight));
                              });

    glfwSetWindowCloseCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow)
                               {
                                   const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(SWindowCloseEvent());
                               });

    glfwSetWindowFocusCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow, const int InFocused)
                               {
                                   const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(SWindowFocusEvent(InFocused));
                               });

    glfwSetWindowIconifyCallback(m_glfwWindow,
                                 [](GLFWwindow* InWindow, int)
                                 {
                                     const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                     Callback(SWindowMinimizeEvent());
                                 });

    glfwSetWindowMaximizeCallback(m_glfwWindow,
                                  [](GLFWwindow* InWindow, int)
                                  {
                                      const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                      Callback(SWindowMaximizeEvent());
                                  });

    glfwSetKeyCallback(m_glfwWindow,
                       [](GLFWwindow* InWindow, const int InKey, const int InScancode, const int InAction, const int InMods)
                       {
                           const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                           Callback(SKeyboardClickEvent(InKey, InScancode, InAction, InMods));
                       });

    glfwSetMouseButtonCallback(m_glfwWindow,
                               [](GLFWwindow* InWindow, const int InButton, const int InAction, const int InMods)
                               {
                                   const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                   Callback(SMouseClickEvent(InButton, InAction, InMods));
                               });

    glfwSetCursorPosCallback(m_glfwWindow,
                             [](GLFWwindow* InWindow, const double InX, const double InY)
                             {
                                 const SEventCallback& Callback = *static_cast<SEventCallback*>(glfwGetWindowUserPointer(InWindow));

                                 double CurrentMouseMove1, CurrentMouseMove2;
                                 glfwGetCursorPos(InWindow, &CurrentMouseMove1, &CurrentMouseMove2);

                                 Callback(SMouseMoveEvent(InX, InY));
                             });
}

SWindow::~SWindow()
{
    glfwDestroyWindow(m_glfwWindow);

    glfwTerminate();
}

GLFWwindow* SWindow::ToGlfw() const
{
    return m_glfwWindow;
}

// #TODO : Add class TickableObject so app can find these classes and then iterate and tick.
void SWindow::Tick(const double InDeltaTime)
{
}

void SWindow::ProcessEvents()
{
    glfwPollEvents();
}

void SWindow::Minimise()
{
    glfwIconifyWindow(m_glfwWindow);
}

void SWindow::Maximise()
{
    glfwMaximizeWindow(m_glfwWindow);
}

} // namespace South