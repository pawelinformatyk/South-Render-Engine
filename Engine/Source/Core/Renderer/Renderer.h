#pragma once

#include "Core/Renderer/RendererContext.h"

namespace South
{
    class VertexIndexBuffer;
    class UniformBuffer;

    // Should be able to be initialised, deinitialised and render objects (using commands).
    // It decides for itself whether renderer should be vulkan, opengl, directx...
    class Renderer
    {
    public:
        static void Init();
        static void Deinit();

        static void BeginFrame();

        // static void Submit(std::function<void()>&);

        static void RenderExampleScene();

        static void RenderQuad(const VkCommandBuffer InCommandBuffer,
                               const VkPipelineLayout InPipelineLayout,
                               const glm::mat4& InTransform);
        static void RenderTriangle(const VkCommandBuffer InCommandBuffer,
                                   const VkPipelineLayout InPipelineLayout,
                                   const glm::mat4& InTransform);
        static void RenderMesh(const VkCommandBuffer InCommandBuffer,
                               const VkPipelineLayout InPipelineLayout,
                               const VertexIndexBuffer& InMeshBuffer,
                               const glm::mat4& InTransform);

        static void EndFrame();

        // Q submit etc.
        static void Present();

        static RendererContext& GetContext() { return *s_Context; };

    private:
        static void LoadExampleScene();

        inline static RendererContext* s_Context = nullptr;

        // Image from swapchain.
        inline static uint32_t s_ImageIndex = 0;

        inline static VertexIndexBuffer* s_QuadModelBuffer    = nullptr;
        inline static UniformBuffer* s_CameraUniformBuffer          = nullptr;
        inline static VertexIndexBuffer* s_ExampleSceneBuffer = nullptr;
    };
} // namespace South