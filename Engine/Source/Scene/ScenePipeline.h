#pragma once

#include "vulkan/vulkan_core.h"

namespace South
{

class ScenePipeline
{
public:
    void Init();

protected:
    VkPipelineLayout m_ViewportPipelineLayout       = nullptr;
    VkPipeline       m_ViewportGraphicsPipelineMesh = nullptr;
};

} // namespace South