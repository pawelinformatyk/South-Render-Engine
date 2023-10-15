#include "sthpch.h"

#include "Example/VulkanExampleApp.h"
#include "Math/VectorTest.h"

int main(int, char**)
{
    South::Logger::Init();

    STH_INFO("Application Start");

    South::Test();

    auto* App = new South::Application;

    App->Run();

    delete App;

    STH_INFO("Application End");

    South::Logger::DeInit();

    return 0;
}