#pragma once

#include "spdlog/spdlog.h"

namespace South
{

class Logger
{
public:
    static void Init();
    static void DeInit();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger()
    {
        return s_CoreLogger;
    }
    static std::shared_ptr<spdlog::logger>& GetVulkanLogger()
    {
        return s_VulkanLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_VulkanLogger;
};

} // namespace South

#define STH_TRACE(...) South::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define STH_INFO(...) South::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define STH_WARN(...) South::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define STH_ERR(...) South::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define STH_CRIT(...) South::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define STH_VK_TRACE(...) South::Logger::GetVulkanLogger()->trace(__VA_ARGS__)
#define STH_VK_INFO(...) South::Logger::GetVulkanLogger()->info(__VA_ARGS__)
#define STH_VK_WARN(...) South::Logger::GetVulkanLogger()->warn(__VA_ARGS__)
#define STH_VK_ERR(...) South::Logger::GetVulkanLogger()->error(__VA_ARGS__)
#define STH_VK_CRIT(...) South::Logger::GetVulkanLogger()->critical(__VA_ARGS__)
