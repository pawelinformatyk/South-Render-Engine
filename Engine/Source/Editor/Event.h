#pragma once

namespace South
{

class Event;

using EventCallback = std::function<void(const Event&)>;

class Event
{
public:
    virtual ~Event() = default;

    template<class T>
    bool IsA() const
    {
        return (dynamic_cast<const T*>(this) != nullptr);
    }

    virtual std::string ToString() const = 0;
};

class WindowMoveEvent final : public Event
{
public:
    WindowMoveEvent(int InX, int InY);

    virtual std::string ToString() const override;
};

class WindowSizeEvent final : public Event
{
public:
    WindowSizeEvent(int InWidth, int InHeight);

    virtual std::string ToString() const override;

    int m_Width  = 0;
    int m_Height = 0;
};

class WindowCloseEvent final : public Event
{
public:
    WindowCloseEvent();

    virtual std::string ToString() const override;
};

class WindowFocusEvent final : public Event
{
public:
    explicit WindowFocusEvent(bool InbFocused);

    virtual std::string ToString() const override;
};

class WindowMinimizeEvent final : public Event
{
public:
    explicit WindowMinimizeEvent();

    virtual std::string ToString() const override;
};

class WindowMaximizeEvent final : public Event
{
public:
    explicit WindowMaximizeEvent();

    virtual std::string ToString() const override;
};

class KeyClickEvent final : public Event
{
public:
    KeyClickEvent(int InKey, int InScancode, int InAction, int InMods);

    virtual std::string ToString() const override;
};

class MouseClickEvent final : public Event
{
public:
    MouseClickEvent(int InButton, int InAction, int InMods);

    virtual std::string ToString() const override;
};

class MouseMoveEvent final : public Event
{
public:
    MouseMoveEvent(double InX, double InY);

    virtual std::string ToString() const override;

    double m_MousePosX = 0.;
    double m_MousePosY = 0.;
};

} // namespace South
