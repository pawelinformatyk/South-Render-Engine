#include "Event.h"
#include <GLFW/glfw3.h>

namespace South
{

std::string SWindowSizeEvent::ToString() const
{
    return "WindowSizeEvent";
}

std::string SWindowCloseEvent::ToString() const
{
    return "WindowCloseEvent";
}

std::string SWindowFocusEvent::ToString() const
{
    return "WindowFocusEvent";
}

std::string SWindowMinimizeEvent::ToString() const
{
    return "WindowMinimizeEvent";
}

std::string SWindowMaximizeEvent::ToString() const
{
    return "WindowMaximizeEvent";
}

std::string SClickEvent::ToString() const
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

std::string SKeyboardClickEvent::ToString() const
{
    return "KeyboardClickEvent " + SClickEvent::ToString();
}

std::string SMouseClickEvent::ToString() const
{
    return "MouseClickEvent";
}

SMouseMoveEvent::SMouseMoveEvent(const double InX, const double InY) : m_MousePosX(InX), m_MousePosY(InY)
{
}

std::string SMouseMoveEvent::ToString() const
{
    return "MouseMoveEvent X:" + std::to_string(m_MousePosX) + " Y:" + std::to_string(m_MousePosY);
}

SWindowMoveEvent::SWindowMoveEvent(int InX, int InY)
{
}

std::string SWindowMoveEvent::ToString() const
{
    return "WindowMoveEvent";
}

SWindowSizeEvent::SWindowSizeEvent(const int InWidth, const int InHeight) : m_Width(InWidth), m_Height(InHeight)
{
}

SWindowCloseEvent::SWindowCloseEvent()
{
}

SWindowFocusEvent::SWindowFocusEvent(bool InbFocused)
{
}

SWindowMinimizeEvent::SWindowMinimizeEvent()
{
}

SWindowMaximizeEvent::SWindowMaximizeEvent()
{
}

int SClickEvent::GetKey() const
{
    return Key;
}

int SClickEvent::GetAction() const
{
    return Action;
}

int SClickEvent::GetInMods() const
{
    return InMods;
}

int SKeyboardClickEvent::GetScancode() const
{
    return Scancode;
}


} // namespace South
