#pragma once

#include "Core/Buffers/UniformBuffer.h"
#include "Editor/Camera.h"
#include "Scene.h"
#include "SceneDescriptorSet.h"
#include "ScenePipeline.h"
#include "SceneRenderPass.h"

// #TODO: Forward declaration sizeof problem

namespace South
{

// #TODO_P: Change some things, and call recreate - potential workflow

/**
 * Renders a scene in a specified way and stores the result texture.
 *
 * The way is defined by shaders, drawing vertex method, camera position etc.
 */
class SSceneViewport
{
public:
    explicit SSceneViewport(std::weak_ptr<const SScene> Scene, const SShader& VertexShader, const SShader& FragmentShader);

    virtual ~SSceneViewport() = default;

    virtual void OnEvent();

    void RecreatePipeline(const SShader& VertexShader, const SShader& FragmentShader);

protected:
    const std::weak_ptr<const SScene> Scene;

    std::unique_ptr<SUniformBuffer>      CameraBuffer;
    std::unique_ptr<SSceneDescriptorSet> DescriptorSet;
    std::unique_ptr<SSceneRenderPass>    RenderPass;
    std::unique_ptr<SScenePipeline>      Pipeline;

    SCamera Camera;
};

} // namespace South