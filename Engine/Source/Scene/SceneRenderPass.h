#pragma once

#include "Core/Renderer/RenderPass.h"

namespace South
{

/**
 *
 */
class SSceneRenderPass
{
public:
     SSceneRenderPass();
    ~SSceneRenderPass();

    VkRenderPass Get() const;

protected:
    VkRenderPass RenderPass = nullptr;
};

} // namespace South