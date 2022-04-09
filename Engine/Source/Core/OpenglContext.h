#pragma once

#include "Context.h"

namespace South
{
    class OpenglContext : public Context
    {
      public:
        virtual void Init() override{};
        virtual void DeInit() override{};
        virtual void Tick() override{};
    };
} // namespace South