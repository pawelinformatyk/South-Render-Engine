#include "SceneViewport.h"


namespace South
{


SSceneViewport::SSceneViewport(std::weak_ptr<const SScene> Scene, const SShader& VertexShader, const SShader& FragmentShader) :
    Scene(std::move(Scene)), DescriptorSet(std::make_unique<SSceneDescriptorSet>()), RenderPass(std::make_unique<SSceneRenderPass>()),
    Pipeline(std::make_unique<SScenePipeline>(*RenderPass, *DescriptorSet, VertexShader, FragmentShader))

{
    // const VkDescriptorBufferInfo CameraBufferInfo {
    //     .buffer = CameraBuffer->GetBuffer(),
    //     .offset = 0,
    //     .range  = sizeof(SUniformBufferObject),
    // };
    //
    // const VkDescriptorImageInfo ViewportImageInfo {
    //     .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    //     .imageView   = ImageView,
    //     .sampler     = TextureSampler,
    // };
    //
    // DescriptorSet->UpdateSets(CameraBufferInfo, ViewportImageInfo);
}

void SSceneViewport::OnEvent()
{
}

void SSceneViewport::RecreatePipeline(const SShader& VertexShader, const SShader& FragmentShader)
{
    Pipeline = std::make_unique<SScenePipeline>(*RenderPass, *DescriptorSet, VertexShader, FragmentShader);
}

} // namespace South