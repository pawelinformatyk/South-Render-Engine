cmake_minimum_required(VERSION 3.28)

project(SouthRenderEngine)

set(CMAKE_CXX_STANDARD 23)

include_directories(SouthRenderEngine Engine/Source)

add_executable(SouthRenderEngine
        Engine/Source/Application/Main.cpp
        Engine/Source/Application/VulkanExampleApp.cpp
        Engine/Source/Application/VulkanExampleApp.h
        Engine/Source/Core/Buffers/Buffer.cpp
        Engine/Source/Core/Buffers/Buffer.h
        Engine/Source/Core/Buffers/UniformBuffer.cpp
        Engine/Source/Core/Buffers/UniformBuffer.h
        Engine/Source/Core/Buffers/VertexIndexBuffer.cpp
        Engine/Source/Core/Buffers/VertexIndexBuffer.h
        Engine/Source/Core/Devices/GraphicCard.cpp
        Engine/Source/Core/Devices/GraphicCard.h
        Engine/Source/Core/Events/EventsInterface.h
        Engine/Source/Core/FileSystem/FileSystem.cpp
        Engine/Source/Core/FileSystem/FileSystem.h
        Engine/Source/Core/Logger/Logger.cpp
        Engine/Source/Core/Logger/Logger.h
        Engine/Source/Core/Math/Vector.cpp
        Engine/Source/Core/Math/Vector.h
        Engine/Source/Core/Mesh/Mesh.cpp
        Engine/Source/Core/Mesh/Mesh.h
        Engine/Source/Core/Mesh/tiny_obj_loader.h
        Engine/Source/Core/Renderer/CommandPool.cpp
        Engine/Source/Core/Renderer/CommandPool.h
        Engine/Source/Core/Renderer/Renderer.cpp
        Engine/Source/Core/Renderer/Renderer.h
        Engine/Source/Core/Renderer/RendererContext.cpp
        Engine/Source/Core/Renderer/RendererContext.h
        Engine/Source/Core/Renderer/RenderPass.cpp
        Engine/Source/Core/Renderer/RenderPass.h
        Engine/Source/Core/Shaders/Shader.cpp
        Engine/Source/Core/Shaders/Shader.h
        Engine/Source/Core/Shaders/ShadersLibrary.cpp
        Engine/Source/Core/Shaders/ShadersLibrary.h
        Engine/Source/Core/Utils/VulkanUtils.cpp
        Engine/Source/Core/Utils/VulkanUtils.h
        Engine/Source/Core/Window/SwapChain.cpp
        Engine/Source/Core/Window/SwapChain.h
        Engine/Source/Core/Window/Window.cpp
        Engine/Source/Core/Window/Window.h
        Engine/Source/Editor/Camera.cpp
        Engine/Source/Editor/Camera.h
        Engine/Source/Editor/Editor.cpp
        Engine/Source/Editor/EditorViewport.cpp
        Engine/Source/Editor/EditorViewport.h
        Engine/Source/Editor/Event.cpp
        Engine/Source/Editor/Event.h
        Engine/ThirdParty/ImGui/imconfig.h
        Engine/ThirdParty/ImGui/imgui.cpp
        Engine/ThirdParty/ImGui/imgui.h
        Engine/ThirdParty/ImGui/imgui_demo.cpp
        Engine/ThirdParty/ImGui/imgui_draw.cpp
        Engine/ThirdParty/ImGui/imgui_internal.h
        Engine/ThirdParty/ImGui/imgui_tables.cpp
        Engine/ThirdParty/ImGui/imgui_widgets.cpp
        Engine/ThirdParty/ImGui/imstb_rectpack.h
        Engine/ThirdParty/ImGui/imstb_textedit.h
        Engine/ThirdParty/ImGui/imstb_truetype.h
        Engine/ThirdParty/ImGui/backends/imgui_impl_glfw.cpp
        Engine/ThirdParty/ImGui/backends/imgui_impl_vulkan.cpp
        Engine/Source/Core/ScenePipeline/ScenePipeline.cpp
        Engine/Source/Core/ScenePipeline/ScenePipeline.h
        Engine/Source/Scene/SceneObject.cpp
        Engine/Source/Scene/SceneObject.h
        Engine/Source/Scene/ScenePipeline.cpp
        Engine/Source/Scene/ScenePipeline.h
)

target_precompile_headers(SouthRenderEngine PRIVATE Engine/Source/sthpch.h)

# GLFW
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build shared libraries" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation" FORCE)
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target" FORCE)
set(GLFW_LIBRARY_TYPE "STATIC" CACHE INTERNAL "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)" FORCE)

add_subdirectory(Engine/ThirdParty/glfw)
include_directories(Engine/ThirdParty/glfw/include)
target_link_libraries(SouthRenderEngine PRIVATE glfw)

target_compile_definitions(SouthRenderEngine PRIVATE GLFW_INCLUDE_VULKAN)

# GLM
set(GLM_BUILD_LIBRARY ON CACHE INTERNAL "Build dynamic/static library" FORCE)
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "Build the test programs" FORCE)
set(GLM_BUILD_INSTALL OFF CACHE INTERNAL "Generate the install target" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE INTERNAL "Enable C++ 20" FORCE)

add_subdirectory(Engine/ThirdParty/glm)
include_directories(Engine/ThirdParty/glm/glm)
target_link_libraries(SouthRenderEngine PRIVATE glm)

target_compile_definitions(SouthRenderEngine PRIVATE GLM_ENABLE_EXPERIMENTAL)

# VULKAN
find_package(Vulkan REQUIRED)
target_link_libraries(SouthRenderEngine PRIVATE Vulkan::Vulkan)

# ImGui
include_directories(Engine/ThirdParty/ImGui)

# SHADERC
set(SHADERC_SKIP_TESTS ON CACHE INTERNAL "Skip building tests" FORCE)
set(SHADERC_SKIP_EXAMPLES ON CACHE INTERNAL "Skip building examples" FORCE)

add_subdirectory(Engine/ThirdParty/shaderc)
include_directories(Engine/ThirdParty/shaderc/libshaderc)
target_link_libraries(SouthRenderEngine PRIVATE shaderc)

# SPDLOG
add_subdirectory(Engine/ThirdParty/spdlog)
include_directories(Engine/ThirdParty/spdlog/include)
target_link_libraries(SouthRenderEngine PRIVATE spdlog)

# DEFINES
target_compile_definitions(SouthRenderEngine PRIVATE TINYOBJLOADER_IMPLEMENTATION)

# Shaders 
# #TODO: Should be always copied
add_custom_command(TARGET SouthRenderEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Engine/Shaders/ $<TARGET_FILE_DIR:SouthRenderEngine>/Engine/Shaders)

# Models, fonts etc
add_custom_command(TARGET SouthRenderEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Engine/Resources/ $<TARGET_FILE_DIR:SouthRenderEngine>/Engine/Resources)