#pragma once

#include "Context.h"

namespace South
{
    class OpenglContext : public Context
    {
      public:
        void Init(GLFWwindow& window) override;

        void DeInit() override;

        void Tick() override;
    };
} // namespace South