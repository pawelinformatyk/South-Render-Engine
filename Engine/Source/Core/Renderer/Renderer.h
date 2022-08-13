#pragma once

#include "Core/Renderer/RendererContext.h"

namespace South
{

    // Should be able to be initialised, deinitialised and render objects (using commands).
    // It decides for itself whether renderer should be vulkan, opengl, directx...
    class Renderer
    {
    public:
        static void Init();
        static void Deinit();

        static void BeginFrame();

        // static void Submit(std::function<void()>&);

        static void DrawExampleScene();

        static void DrawQuad();
        static void DrawTriangle();

        static void EndFrame();

        // Q submit etc.
        static void Present();

        static RendererContext& GetContext() { return *s_Context; };

    private:
        inline static RendererContext* s_Context = nullptr;

        // Image from swapchain.
        inline static uint32_t s_ImageIndex = 0;

        inline static class VertexIndexBuffer* s_QuadModelBuffer = nullptr;
    };

} // namespace South