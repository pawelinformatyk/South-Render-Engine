#include "sthpch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
    South::Logger::Init();

    STH_INFO("Application Start");

    auto& App = South::Application::Get();

    App.Init();
    App.Run();
    App.DeInit();

    STH_INFO("Application Shutdown.");

    South::Logger::DeInit();

    return 0;
}
