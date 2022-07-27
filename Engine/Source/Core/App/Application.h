#pragma once

#include "Core/Window/Window.h"

namespace South
{
    class VulkanContext;

    // Input, window resize,close,open, events,
    // run loop, what decides what keep app going
    class Application
    {
    public:
        Application();

        void Init();

        void Run();

        void DeInit();

        void OnEvent();

        Window& GetWindow() const;

    private:
        void InitGUI();
        void DrawGUI();
        void DeInitGUI();

        void CloseWindow();

        std::unique_ptr<Window> m_Window = nullptr;

        bool m_bRunning = false;

        // Static functions
    public:
        static const char* GetName();
        static Application& Get();

        static void Kaboom();

        static inline Application* s_Instance;
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