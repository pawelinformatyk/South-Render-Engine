#pragma once

#include "Editor/Camera.h"
#include "Scene.h"
#include "SceneDescriptorSet.h"
#include "ScenePipeline.h"
#include "SceneRenderPass.h"

// #TODO: Forward declaration sizeof problem

namespace South
{

class SSceneViewport
{
public:
    explicit SSceneViewport(std::weak_ptr<const SScene> Scene, const SShader& VertexShader, const SShader& FragmentShader);

    virtual ~SSceneViewport() = default;

    virtual void OnEvent();

    void RecreatePipeline(const SShader& VertexShader, const SShader& FragmentShader);

protected:
    const std::weak_ptr<const SScene> Scene;

    std::unique_ptr<SSceneDescriptorSet> DescriptorSet;
    std::unique_ptr<SSceneRenderPass>    RenderPass;
    std::unique_ptr<SScenePipeline>      Pipeline;

    SCamera Camera;
};

} // namespace South