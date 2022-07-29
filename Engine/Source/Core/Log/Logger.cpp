#include "sthpch.h"

#include "Core/Log/Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace South
{

    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_VulkanLogger;

    void Logger::Init()
    {
        // std::vector<spdlog::sink_ptr> hazelSinks = { std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        //                                              std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        //                                                  "logs/HAZEL.log", true) };

        // std::vector<spdlog::sink_ptr> appSinks = { std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        //                                            std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log",
        //                                                                                                true),
        //                                            std::make_shared<EditorConsoleSink>(1) };

        // std::vector<spdlog::sink_ptr> editorConsoleSinks = { std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        //                                                          "logs/APP.log", true),
        //                                                      std::make_shared<EditorConsoleSink>(1) };

        // hazelSinks[0]->set_pattern("%^[%T] %n: %v%$");
        // hazelSinks[1]->set_pattern("[%T] [%l] %n: %v");
        // appSinks[0]->set_pattern("%^[%T] %n: %v%$");
        // appSinks[1]->set_pattern("[%T] [%l] %n: %v");
        // appSinks[2]->set_pattern("%^[%T] %n: %v%$");

        // for (auto sink : editorConsoleSinks) sink->set_pattern("%^[%T] %n: %v%$");
        // editorConsoleSinks[0]->set_pattern("[%T] [%l] %n: %v");

        // s_CoreLogger = std::make_shared<spdlog::logger>("HAZEL", hazelSinks.begin(), hazelSinks.end());
        // s_CoreLogger->set_level(spdlog::level::trace);

        // s_VulkanLogger = std::make_shared<spdlog::logger>("APP", appSinks.begin(), appSinks.end());
        // s_VulkanLogger->set_level(spdlog::level::trace);

        // s_EditorConsoleLogger =
        //     std::make_shared<spdlog::logger>("Console", editorConsoleSinks.begin(), editorConsoleSinks.end());
        // s_EditorConsoleLogger->set_level(spdlog::level::trace);
    }

    void Logger::DeInit()
    {
        s_VulkanLogger.reset();
        s_CoreLogger.reset();
        spdlog::drop_all();
    }

} // namespace South
