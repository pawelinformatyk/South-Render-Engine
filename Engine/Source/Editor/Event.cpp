#include "Event.h"
#include <GLFW/glfw3.h>

namespace South
{

std::string WindowSizeEvent::ToString() const
{
    return "WindowSizeEvent";
}

std::string WindowCloseEvent::ToString() const
{
    return "WindowCloseEvent";
}

std::string WindowFocusEvent::ToString() const
{
    return "WindowFocusEvent";
}

std::string WindowMinimizeEvent::ToString() const
{
    return "WindowMinimizeEvent";
}

std::string WindowMaximizeEvent::ToString() const
{
    return "WindowMaximizeEvent";
}

std::string ClickEvent::ToString() const
{
    std::string OutStr;

    const char* KeyName = glfwGetKeyName(Key, 0);
    if(!KeyName)
    {
        return OutStr;
    }

    OutStr = KeyName;

    if(Action == GLFW_RELEASE)
    {
        OutStr += " Release";
    }
    else if(Action == GLFW_PRESS)
    {
        OutStr += " Press";
    }
    else if(Action == GLFW_REPEAT)
    {
        OutStr += " Repeat";
    }

    return OutStr;
}

std::string KeyboardClickEvent::ToString() const
{
    return "KeyboardClickEvent " + ClickEvent::ToString();
}

std::string MouseClickEvent::ToString() const
{
    return "MouseClickEvent";
}

MouseMoveEvent::MouseMoveEvent(const double InX, const double InY) : m_MousePosX(InX), m_MousePosY(InY)
{
}

std::string MouseMoveEvent::ToString() const
{
    return "MouseMoveEvent X:" + std::to_string(m_MousePosX) + " Y:" + std::to_string(m_MousePosY);
}

WindowMoveEvent::WindowMoveEvent(int InX, int InY)
{
}

std::string WindowMoveEvent::ToString() const
{
    return "WindowMoveEvent";
}

WindowSizeEvent::WindowSizeEvent(const int InWidth, const int InHeight) : m_Width(InWidth), m_Height(InHeight)
{
}

WindowCloseEvent::WindowCloseEvent()
{
}

WindowFocusEvent::WindowFocusEvent(bool InbFocused)
{
}

WindowMinimizeEvent::WindowMinimizeEvent()
{
}

WindowMaximizeEvent::WindowMaximizeEvent()
{
}

int ClickEvent::GetKey() const
{
    return Key;
}

int ClickEvent::GetAction() const
{
    return Action;
}

int ClickEvent::GetInMods() const
{
    return InMods;
}

int KeyboardClickEvent::GetScancode() const
{
    return Scancode;
}


} // namespace South
