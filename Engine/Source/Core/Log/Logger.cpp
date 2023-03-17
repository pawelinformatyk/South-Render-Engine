#include "sthpch.h"

#include "Core/Log/Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace South
{

std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::s_VulkanLogger;

void Logger::Init()
{
    s_CoreLogger = spdlog::stdout_color_mt("CORE", spdlog::color_mode::always);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->set_pattern("[%H:%M:%S] %^[%n] [%l] %v%$");

    s_VulkanLogger = spdlog::stdout_color_mt("VULKAN", spdlog::color_mode::always);
    s_VulkanLogger->set_level(spdlog::level::trace);
    s_VulkanLogger->set_pattern("[%H:%M:%S] %^[%n] [%l] %v%$");
}

void Logger::DeInit()
{
    s_VulkanLogger.reset();
    s_CoreLogger.reset();
    spdlog::drop_all();
}

} // namespace South
