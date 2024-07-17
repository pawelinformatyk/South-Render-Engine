#include "Event.h"

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

std::string KeyClickEvent::ToString() const
{
    return "KeyClickEvent";
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

KeyClickEvent::KeyClickEvent(int InKey, int InScancode, int InAction, int InMods)
{
}

MouseClickEvent::MouseClickEvent(int InButton, int InAction, int InMods)
{
}


} // namespace South
