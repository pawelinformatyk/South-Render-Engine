#pragma once

struct GLFWwindow;

namespace South
{
    class Context
    {
      public:
        virtual void Init()   = 0;
        virtual void DeInit() = 0;
        virtual void Tick()   = 0;
    };
}; // namespace South