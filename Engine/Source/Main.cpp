#include "sthpch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
    South::Logger::Init();

    STH_TRACE("Application Start");

    auto& App = South::Application::Get();

    App.Init();
    App.Run();
    App.DeInit();

    STH_TRACE("Application Shutdown.");

    South::Logger::DeInit();

    return 0;
}
