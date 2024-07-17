#pragma once

namespace South
{
    class IEvents
    {
        void OnKeyPressed(int Key, int Scancode, int Action, int Mods);
        void OnIconifiedWindow(int bIconified);
        void OnMaximiseWindow();
    };
} // namespace South