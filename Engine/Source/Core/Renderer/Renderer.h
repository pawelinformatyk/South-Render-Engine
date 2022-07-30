#pragma once

#include "Core/Renderer/RendererContext.h"

namespace South
{

    enum ERenderingAPI
    {
        Vulkan,
        DirectX,
        OpenGL,
    };

    // Should be able to be initialised, deinitialised and render objects (using commands).
    // It decides for itself whether renderer should be vulkan, opengl, directx...
    class Renderer
    {
    public:
        static void Init();
        static void DeInit();

        static void BeginFrame();

        static void Submit(std::function<void()>&);
        static void Draw();

        static void EndFrame();

        // Q submit etc.
        static void Flush();

        static class RendererContext& GetContext() { return *s_Context; };

    private:
        inline static RendererContext* s_Context = nullptr;

        inline static uint32_t s_ImageIndex = 0;
    };

} // namespace South