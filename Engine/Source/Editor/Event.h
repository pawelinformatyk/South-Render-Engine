#pragma once

namespace South
{

class SEvent;

using SEventCallback = std::function<void(const SEvent&)>;

class SEvent
{
public:
    virtual ~SEvent() = default;

    template<class T>
    bool IsA() const
    {
        return (dynamic_cast<const T*>(this) != nullptr);
    }

    virtual std::string ToString() const = 0;
};

class SWindowMoveEvent final : public SEvent
{
public:
    SWindowMoveEvent(int InX, int InY);

    virtual std::string ToString() const override;
};

class SWindowSizeEvent final : public SEvent
{
public:
    SWindowSizeEvent(int InWidth, int InHeight);

    virtual std::string ToString() const override;

    int m_Width  = 0;
    int m_Height = 0;
};

class SWindowCloseEvent final : public SEvent
{
public:
    SWindowCloseEvent();

    virtual std::string ToString() const override;
};

class SWindowFocusEvent final : public SEvent
{
public:
    explicit SWindowFocusEvent(bool InbFocused);

    virtual std::string ToString() const override;
};

class SWindowMinimizeEvent final : public SEvent
{
public:
    explicit SWindowMinimizeEvent();

    virtual std::string ToString() const override;
};

class SWindowMaximizeEvent final : public SEvent
{
public:
    explicit SWindowMaximizeEvent();

    virtual std::string ToString() const override;
};

class SClickEvent : public SEvent
{
public:
    SClickEvent(int InKey, int InAction, int InMods) : Key(InKey), Action(InAction), InMods(InMods)
    {
    }

    int GetKey() const;
    int GetAction() const;
    int GetInMods() const;

    virtual std::string ToString() const override;

protected:
    int Key    = 0;
    int Action = 0;
    int InMods = 0;
};

class SKeyboardClickEvent final : public SClickEvent
{
public:
    SKeyboardClickEvent(int InKey, int InScancode, int InAction, int InMods) : SClickEvent(InKey, InAction, InMods), Scancode(InScancode)
    {
    }

    virtual std::string ToString() const override;

    int GetScancode() const;

private:
    int Scancode = 0;
};

class SMouseClickEvent final : public SClickEvent
{
public:
    SMouseClickEvent(int InKey, int InAction, int InMods) : SClickEvent(InKey, InAction, InMods)
    {
    }

    virtual std::string ToString() const override;
};

class SMouseMoveEvent final : public SEvent
{
public:
    SMouseMoveEvent(double InX, double InY);

    virtual std::string ToString() const override;

    double m_MousePosX = 0.;
    double m_MousePosY = 0.;
};

} // namespace South
