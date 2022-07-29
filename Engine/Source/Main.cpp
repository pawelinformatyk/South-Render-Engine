#include "sthpch.h"

#include "Core/App/Application.h"

int main(int argc, char** argv)
{
    South::Logger::Init();

    STH_INFO("Application Start");
    STH_INFO("");

    auto* App = new South::Application;

    App->Run();

    delete App;

    STH_INFO("");
    STH_INFO("Application Shutdown.");

    South::Logger::DeInit();

    return 0;
}