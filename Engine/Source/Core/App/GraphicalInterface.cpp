#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/App/GraphicalInterface.h"
#include "Core/Renderer/Renderer.h"
#include "Core/VulkanDevice.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace South
{

    void GraphicalInterface::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& IO = ImGui::GetIO();

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        ImGui_ImplGlfw_InitForVulkan(Application::Get().GetWindow().GetglfwWindow(), true);

        const RendererContext& Context = Renderer::GetContext();

        const VulkanDevice& GPU = Context.GetGpuDevice();

        const VkDescriptorPoolSize PoolSizes[] = { { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
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

        const VkDescriptorPoolCreateInfo PoolInfo = {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets       = 1000 * IM_ARRAYSIZE(PoolSizes),
            .poolSizeCount = (uint32_t)IM_ARRAYSIZE(PoolSizes),
            .pPoolSizes    = PoolSizes,
        };

        VkDescriptorPool DescriptorPool;
        vkCreateDescriptorPool(GPU.GetDevice(), &PoolInfo, VK_NULL_HANDLE, &DescriptorPool);

        // #TODO : Remove this struct later.
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

        SetupStyleAndFonts();
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
        const ImGuiViewport* Viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(Viewport->Pos);
        ImGui::SetNextWindowSize(Viewport->Size);

        const ImGuiWindowFlags TitleBarFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
                                               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                                               ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
                                               ImGuiWindowFlags_NoDocking;

        ImGui::Begin("TitleBar", nullptr, TitleBarFlags);
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
                if (ImGui::Button("_")) { Application::Get().Minimise(); }

                if (ImGui::Button("[]")) { Application::Get().Maximise(); }

                if (ImGui::Button("X")) { Application::Get().Close(); }
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();

        ImGui::ShowDemoWindow();
    }

    void GraphicalInterface::EndFrame()
    {
        ImGui::Render();

        ImDrawData* DrawData = ImGui::GetDrawData();

        ImGui_ImplVulkan_RenderDrawData(DrawData, Renderer::GetContext().GetCommandBuffer());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    void GraphicalInterface::SetupStyleAndFonts()
    {
        ImGuiIO& IO = ImGui::GetIO();

        ImVec4* Colors                         = ImGui::GetStyle().Colors;
        Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        Colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        Colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        Colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        Colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        Colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
        Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
        Colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        Colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        Colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        Colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        Colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        Colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        Colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        Colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        Colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        Colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        Colors[ImGuiCol_DockingPreview]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        Colors[ImGuiCol_DockingEmptyBg]        = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_PlotHistogram]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        Colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        Colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        Colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        Colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        Colors[ImGuiCol_NavHighlight]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& Style       = ImGui::GetStyle();
        Style.WindowPadding     = ImVec2(8.00f, 8.00f);
        Style.FramePadding      = ImVec2(5.00f, 2.00f);
        Style.CellPadding       = ImVec2(6.00f, 6.00f);
        Style.ItemSpacing       = ImVec2(6.00f, 6.00f);
        Style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
        Style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        Style.IndentSpacing     = 25;
        Style.ScrollbarSize     = 15;
        Style.GrabMinSize       = 10;
        Style.WindowBorderSize  = 1;
        Style.ChildBorderSize   = 1;
        Style.PopupBorderSize   = 1;
        Style.FrameBorderSize   = 1;
        Style.TabBorderSize     = 1;
        Style.WindowRounding    = 0.f;
        Style.ChildRounding     = 4;
        Style.FrameRounding     = 3;
        Style.PopupRounding     = 4;
        Style.ScrollbarRounding = 9;
        Style.GrabRounding      = 3;
        Style.LogSliderDeadzone = 4;
        Style.TabRounding       = 4;

        // Fonts
        {

            IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\DroidSans.ttf", 17);

            const RendererContext& Context = Renderer::GetContext();
            const VulkanDevice& GPU        = Context.GetGpuDevice();

            VkCommandBuffer CmdBuffer = Context.GetCommandBuffer();
            VkCommandPool CmdPool     = Context.GetCommandPool();

            vkResetCommandPool(GPU.GetDevice(), CmdPool, 0);

            VkCommandBufferBeginInfo BeginInfo = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            };
            BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(CmdBuffer, &BeginInfo);

            ImGui_ImplVulkan_CreateFontsTexture(CmdBuffer);

            const VkSubmitInfo SubmitInfo = {
                .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .commandBufferCount = 1,
                .pCommandBuffers    = &CmdBuffer,
            };

            vkEndCommandBuffer(CmdBuffer);

            vkQueueSubmit(GPU.GetQ(), 1, &SubmitInfo, VK_NULL_HANDLE);

            vkDeviceWaitIdle(GPU.GetDevice());

            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
    }

} // namespace South