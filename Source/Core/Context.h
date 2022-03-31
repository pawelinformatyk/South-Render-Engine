#pragma once

struct GLFWwindow;

namespace South
{
    class Context
    {
      public:
        virtual void Init(GLFWwindow& window) = 0;
        virtual void DeInit()                 = 0;
        virtual void Tick()                   = 0;
    };
}; // namespace South