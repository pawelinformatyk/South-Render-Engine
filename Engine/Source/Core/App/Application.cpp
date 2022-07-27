#include "sthpch.h"

#include "Core/App/Application.h"

#include "Core/VulkanContext.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


namespace South
{

    Application::Application() { s_Instance = this; }

    void Application::Init()
    {
        m_Window = std::make_unique<Window>();
        m_Window->SetCloseWindowCallback([this]() { CloseWindow(); });
        m_Window->Init();

        VulkanContext::Get().Init();

        m_bRunning = true;

        // InitGUI();
    }

    void Application::Run()
    {
        while (m_bRunning)
        {
            m_Window->ProcessEvents();

            VulkanContext::Get().Tick();

            // DrawGUI();
        }
    }

    void Application::DeInit()
    {
        // DeInitGUI();

        VulkanContext::Get().DeInit();

        m_Window->SetCloseWindowCallback([]() {});

        m_Window->DeInit();
    }

    Window& Application::GetWindow() const { return *m_Window; }

    void Application::InitGUI()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(m_Window->GetglfwWindow(), true);

        // #TODO : get that and render pass.
        ImGui_ImplVulkan_InitInfo initInfo /*= {
            .Instance        = g_Instance,
            .PhysicalDevice  = g_PhysicalDevice,
            .Device          = g_Device,
            .QueueFamily     = g_QueueFamily,
            .Queue           = g_Queue,
            .PipelineCache   = g_PipelineCache,
            .DescriptorPool  = g_DescriptorPool,
            .Subpass         = 0,
            .MinImageCount   = g_MinImageCount,
            .ImageCount      = wd->ImageCount,
            .MSAASamples     = VK_SAMPLE_COUNT_1_BIT,
            .Allocator       = g_Allocator,
            .CheckVkResultFn = check_vk_result,
        }*/;

        // ImGui_ImplVulkan_Init(&initInfo, RenderPass);
    }

    void Application::DrawGUI()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("VSouth");
            {
                ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)
            }
            ImGui::End();
        }

        ImGui::Render();

        // #TODO : Create command buffer?
        // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), CommandBuffer);
    }

    void Application::DeInitGUI()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::CloseWindow() { m_bRunning = false; }

    const char* Application::GetName() { return "SouthRenderEngine"; }

    Application& Application::Get() { return *s_Instance; }

    void Application::Kaboom()
    {
        STH_CRIT("Kaboom");

        *(int*)0 = 0;
    }

} // namespace South