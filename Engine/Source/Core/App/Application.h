#pragma once

#include "Core/App/GraphicalInterface.h"
#include "Core/Window/Window.h"

namespace South
{

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

        std::unique_ptr<GraphicalInterface> m_GUI = nullptr;
        std::unique_ptr<Window> m_Window          = nullptr;

        bool m_bRunning = false;

        //~ Events
    public:
        void Minimise();
        void Maximise();
        void Close();

    private:
        void ProcessEvents();

        void OnKeyPressed(int Key, int Scancode, int Action, int Mods);
        void OnIconifiedWindow(int bIconified);
        void OnMaximiseWindow();

        //~ Events.
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