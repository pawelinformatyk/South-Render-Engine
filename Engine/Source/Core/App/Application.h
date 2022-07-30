#pragma once

#include "Core/Window/Window.h"

// #TODO : Init - not deleting memory?

namespace South
{

    // Input, window resize,close,open, events,
    // run loop, what decides what keep app going
    class Application
    {
        // ~Static
    public:
        static const char* GetName() { return "SouthRenderEngine"; }

        static Application& Get() { return *s_Instance; };

        static void Kaboom();

    private:
        static inline Application* s_Instance = nullptr;

        // ~Static
    public:
        Application();
        ~Application();

        void Run();

        Window& GetWindow() const { return *m_Window; }

    private:
        void Init();
        void DeInit();

        void InitGUI();
        void DeInitGUI();

        bool m_bRunning = false;

        //~ Events and Windows.
    public:
        void ProcessEvents();

        void CloseApplication();
        void MaximiseApplication();
        void MinimiseApplication(bool bMinimized);

    private:
        std::unique_ptr<Window> m_Window = nullptr;

        bool m_WindowMinimized = false;

        //~ Events and Windows.
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