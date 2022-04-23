#pragma once

#include "Core/Window.h"

namespace South
{
    class Context;

    const char* GetAppName();

    // Input, window resize,close,open, events,
    // run loop, what decides what keep app going
    class Application
    {
      public:
        void Init();
        void Run();
        void DeInit();

        void OnEvent();

        Window& GetWindow() const;

        static Application& Get();

      private:
        void CloseWindow();

        // Renderer?
        Context* GraphicalContext = nullptr;

        std::unique_ptr<Window> pWindow = nullptr;

        bool bRunning = false;

        // Static functions
      public:
        static void Kaboom();

      private:
        static inline Application* Instance = nullptr;
    };

    // Renderer -> move vulkan functions to this. I mean like instance etc.

    // I want to have multiple viewports. How do I do it. Do I need layers or just
    // two imgui windows with rendered scenes with different camera.
    // VkPipelineViewportStateCreateInfo - > viewport count?

    // Multiple viewports -> multiple textures rendered and shown from different VIEW matrix.
    // I thjink I need to implement vulcan code so i can then decide how to do it.
    // What is framebuffer exactly?

    // Editor layer from cherno -> showcase layer

} // namespace South