#pragma once

#include "Core/Shaders/Shader.h"
#include "vulkan/vulkan_core.h"

namespace South
{
class SSceneDescriptorSet;

class SSceneRenderPass;

/**
 * Pipeline is basically pipe of shaders.
 */
class SScenePipeline
{
public:
     SScenePipeline(const SSceneRenderPass&    RenderPass,
                    const SSceneDescriptorSet& DescriptorSet,
                    const SShader&             VertexShader,
                    const SShader&             FragmentShader);
    ~SScenePipeline();

protected:
    VkPipelineLayout PipelineLayout = nullptr;
    VkPipeline       Pipeline       = nullptr;
};

} // namespace South