#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/Renderer/Renderer.h"
#include "Core/VulkanDevice.h"
#include "GraphicalInterface.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace South
{

    void GraphicalInterface::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        SetStyle();

        ImGui_ImplGlfw_InitForVulkan(Application::Get().GetWindow().GetglfwWindow(), true);

        const RendererContext& Context = Renderer::GetContext();

        const VulkanDevice& GPU = Context.GetCurrentDevice();

        VkDescriptorPoolSize PoolSizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
                                             { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
                                             { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
                                             { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                                             { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                                             { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 } };

        VkDescriptorPoolCreateInfo PoolInfo = {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets       = 1000 * IM_ARRAYSIZE(PoolSizes),
            .poolSizeCount = (uint32_t)IM_ARRAYSIZE(PoolSizes),
            .pPoolSizes    = PoolSizes,
        };

        VkDescriptorPool DescriptorPool;
        vkCreateDescriptorPool(GPU.GetDevice(), &PoolInfo, VK_NULL_HANDLE, &DescriptorPool);

        ImGui_ImplVulkan_InitInfo initInfo = {
            .Instance        = Context.GetVulkanInstance(),
            .PhysicalDevice  = GPU.GetPhysicalDevice(),
            .Device          = GPU.GetDevice(),
            .QueueFamily     = GPU.GetQFamilyIndex(),
            .Queue           = GPU.GetQ(),
            .PipelineCache   = VK_NULL_HANDLE,
            .DescriptorPool  = DescriptorPool,
            .Subpass         = 0,
            .MinImageCount   = 2,
            .ImageCount      = 2,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .Allocator       = VK_NULL_HANDLE,
            .CheckVkResultFn = VK_NULL_HANDLE,
        };

        ImGui_ImplVulkan_Init(&initInfo, Context.GetRenderPass());

        VkCommandBuffer CmdBuffer = Context.GetCommandBuffer();
        VkCommandPool CmdPool     = Context.GetCommandPool();

        vkResetCommandPool(GPU.GetDevice(), CmdPool, 0);

        VkCommandBufferBeginInfo BeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        };
        BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

        ImGui_ImplVulkan_CreateFontsTexture(CmdBuffer);

        VkSubmitInfo EndInfo = {
            .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers    = &CmdBuffer,
        };

        vkEndCommandBuffer(CmdBuffer);

        vkQueueSubmit(GPU.GetQ(), 1, &EndInfo, VK_NULL_HANDLE);

        vkDeviceWaitIdle(GPU.GetDevice());

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void GraphicalInterface::DeInit()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void GraphicalInterface::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GraphicalInterface::Draw()
    {
        // DrawTitleBar();
        ImGui::Begin("TitleBar", nullptr,
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking);
        {

            ImGui::BeginMenuBar();
            {
                ImGui::Separator();

                if (ImGui::BeginMenu("File"))
                {
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");

                    ImGui::EndMenu();
                }
                ImGui::Separator();

                if (ImGui::BeginMenu("Project Settings"))
                {
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::BeginMenu("Editor Settings"))
                {
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");

                    ImGui::EndMenu();
                }

                ImGui::Separator();
                if (ImGui::BeginMenu("View"))
                {
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");
                    ImGui::MenuItem("Option");

                    ImGui::EndMenu();
                }

                ImGui::Separator();
            }
            ImGui::EndMenuBar();

            ImGui::BeginMenuBar();
            {
                if (ImGui::Button("_")) { Application::Get().MinimiseApplication(true); }

                if (ImGui::Button("[]")) { Application::Get().MaximiseApplication(); }

                if (ImGui::Button("X")) { Application::Get().CloseApplication(); }
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    void GraphicalInterface::EndFrame()
    {
        ImGui::Render();

        ImDrawData* DrawData = ImGui::GetDrawData();

        ImGui_ImplVulkan_RenderDrawData(DrawData, Renderer::GetContext().GetCommandBuffer());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void GraphicalInterface::SetStyle()
    {
        ImGuiIO& IO = ImGui::GetIO();

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\DroidSans.ttf", 17);

        ImVec4* colors                         = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_DockingPreview]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg]        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style       = ImGui::GetStyle();
        style.WindowPadding     = ImVec2(8.00f, 8.00f);
        style.FramePadding      = ImVec2(5.00f, 2.00f);
        style.CellPadding       = ImVec2(6.00f, 6.00f);
        style.ItemSpacing       = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing     = 25;
        style.ScrollbarSize     = 15;
        style.GrabMinSize       = 10;
        style.WindowBorderSize  = 1;
        style.ChildBorderSize   = 1;
        style.PopupBorderSize   = 1;
        style.FrameBorderSize   = 1;
        style.TabBorderSize     = 1;
        style.WindowRounding    = 0.f;
        style.ChildRounding     = 4;
        style.FrameRounding     = 3;
        style.PopupRounding     = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding      = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding       = 4;
    }

} // namespace South