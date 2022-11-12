#include "sthpch.h"

#include "Core/App/Application.h"
#include "Core/App/GraphicalInterface.h"
#include "Core/Devices/GraphicCard.h"
#include "Core/Renderer/Renderer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include <filesystem>

namespace South
{

    void GraphicalInterface::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& IO = ImGui::GetIO();

        IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        ImGui_ImplGlfw_InitForVulkan(Application::Get().GetWindow().GetglfwWindow(), true);

        const RendererContext& Context = Renderer::GetContext();
        const GraphicCard& GPU         = Context.GetGraphicCard();
        const Queue& GraphicQueue      = Context.GetGraphicQueue();

        // #TODO : Remove this struct later and refactor ImGui_ImplVulkan_...
        ImGui_ImplVulkan_InitInfo InitInfo = {
            .Instance        = Context.GetVulkanInstance(),
            .PhysicalDevice  = GPU.GetPhysicalDevice(),
            .Device          = Context.GetLogicalDevice(),
            .QueueFamily     = GraphicQueue.m_QueueFamily,
            .Queue           = GraphicQueue.m_Queue,
            .PipelineCache   = VK_NULL_HANDLE,
            .DescriptorPool  = Context.GetDescriptorPool(),
            .Subpass         = 0,
            .MinImageCount   = 2,
            .ImageCount      = 2,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .Allocator       = VK_NULL_HANDLE,
            .CheckVkResultFn = VK_NULL_HANDLE,
        };

        ImGui_ImplVulkan_Init(&InitInfo, Context.GetRenderPass());

        FindAndAddFonts();
        StyleColorsSouth();
    }

    void GraphicalInterface::DeInit()
    {
        vkDeviceWaitIdle(Renderer::GetContext().GetLogicalDevice());

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

    void GraphicalInterface::Show(float FrameTime_Seconds)
    {
        ShowTitlebar();

        ImGui::Begin("Specification & Statistics");
        {
            if (ImGui::BeginTable("Statistics", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                const uint16_t DrawCalls = 1;

                ImGui::TableSetupColumn("Statistics");
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Render Time");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%f ms", FrameTime_Seconds * 1e3);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Draw calls");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%u", DrawCalls);
            }
            ImGui::EndTable();

            if (ImGui::BeginTable("GraphicCard specification", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
            {
                const GraphicCard& GPU                       = Renderer::GetContext().GetGraphicCard();
                const VkPhysicalDeviceProperties& Properties = GPU.GetProperties();

                ImGui::TableSetupColumn("GraphicCard Specification");
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Name");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text(Properties.deviceName);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("ApiVersion");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%u", Properties.apiVersion);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("DriverVersion");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%u", Properties.driverVersion);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("VendorID");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%u", Properties.vendorID);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("DeviceID");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%u", Properties.deviceID);

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("DeviceType");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", GPU.GetTypeName().c_str());

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Limits");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("");

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("SparseProperties");
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("");
            }
            ImGui::EndTable();
        }
        ImGui::End();

        ImGui::ShowDemoWindow();
    }

    void GraphicalInterface::ShowTitlebar() const
    {
        const ImGuiStyle& Style = ImGui::GetStyle();

        const ImGuiViewport* Viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(Viewport->Pos);
        ImGui::SetNextWindowSize(Viewport->Size);

        constexpr ImGuiWindowFlags TitleBarFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                                   ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDocking;

        ImGui::Begin(Application::GetName(), nullptr, TitleBarFlags);

        ImGui::BeginGroup();
        {
            ImGui::BeginMenuBar();
            {
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
                    ImGui::Button("Font");
                    ImGui::Button("Font");
                    ImGui::Button("Font");

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem("Open new window");

                    ImGui::EndMenu();
                }

                ImGui::Separator();
            }
            ImGui::EndMenuBar();

            ImGui::BeginMenuBar();
            {
                const float ControlButtonsWidth = ImGui::CalcTextSize("_").x + ImGui::CalcTextSize("[ ]").x +
                                                  ImGui::CalcTextSize("X").x + Style.ItemSpacing.x * 2.f +
                                                  Style.FramePadding.x * 6.f;

                // Title bar is always size of the window so I can get that instead of using GetCursor...
                ImGui::SetCursorPosX(Viewport->Size.x - ControlButtonsWidth - 6.f);

                if (ImGui::Button("_")) { Application::Get().Minimise(); }

                if (ImGui::Button("[ ]")) { Application::Get().Maximise(); }

                if (ImGui::Button("X")) { Application::Get().Close(); }
            }
            ImGui::EndMenuBar();
        }
        ImGui::EndGroup();

        ImGui::End();
    }

    void GraphicalInterface::FindAndAddFonts()
    {
        ImGuiIO& IO = ImGui::GetIO();

        constexpr float FontSize = 17.f;
        // #TODO : Path should be somewhere coded? 
        IO.FontDefault           = IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\DroidSans.ttf", FontSize);
        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Cousine-Regular.ttf", FontSize);
        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Karla-Regular.ttf", FontSize);
        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyClean.ttf", FontSize);
        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\ProggyTiny.ttf", FontSize);
        IO.Fonts->AddFontFromFileTTF("Resources\\Fonts\\Roboto-Medium.ttf", FontSize);

        const RendererContext& Context = Renderer::GetContext();
        const Queue& GraphicQueue      = Context.GetGraphicQueue();
        VkDevice LogicalDevice         = Context.GetLogicalDevice();

        VkCommandBuffer CmdBuffer = Context.GetCommandBuffer();
        VkCommandPool CmdPool     = Context.GetCommandPool();

        vkResetCommandPool(LogicalDevice, CmdPool, 0);

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

        vkQueueSubmit(GraphicQueue.m_Queue, 1, &SubmitInfo, VK_NULL_HANDLE);

        vkDeviceWaitIdle(LogicalDevice);

        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void GraphicalInterface::StyleColorsSouth()
    {
        ImGuiStyle& Style = ImGui::GetStyle();

        ImVec4* Colors                         = Style.Colors;
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
        Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.4156f, 0.3529f, 0.8039f, 1.00f);
        Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.66f, 0.66f, 0.66f, 0.54f);
        Colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        Colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        Colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
        Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.70f);
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
    }

    void GraphicalInterface::EndFrame()
    {
        ImGui::Render();

        ImDrawData* DrawData = ImGui::GetDrawData();

        ImGui_ImplVulkan_RenderDrawData(DrawData, Renderer::GetContext().GetCommandBuffer());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

} // namespace South