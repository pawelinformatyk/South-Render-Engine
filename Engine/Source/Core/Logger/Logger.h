#pragma once

#include "spdlog/spdlog.h"

namespace South
{

class SLogger
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

#define STH_TRACE(...) South::SLogger::GetCoreLogger()->trace(__VA_ARGS__)
#define STH_INFO(...) South::SLogger::GetCoreLogger()->info(__VA_ARGS__)
#define STH_WARN(...) South::SLogger::GetCoreLogger()->warn(__VA_ARGS__)
#define STH_ERR(...) South::SLogger::GetCoreLogger()->error(__VA_ARGS__)
#define STH_CRIT(...) South::SLogger::GetCoreLogger()->critical(__VA_ARGS__)

#define STH_VK_TRACE(...) South::SLogger::GetVulkanLogger()->trace(__VA_ARGS__)
#define STH_VK_INFO(...) South::SLogger::GetVulkanLogger()->info(__VA_ARGS__)
#define STH_VK_WARN(...) South::SLogger::GetVulkanLogger()->warn(__VA_ARGS__)
#define STH_VK_ERR(...) South::SLogger::GetVulkanLogger()->error(__VA_ARGS__)
#define STH_VK_CRIT(...) South::SLogger::GetVulkanLogger()->critical(__VA_ARGS__)
