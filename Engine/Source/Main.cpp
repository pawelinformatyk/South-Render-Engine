#include "sthpch.h"

#include "Core/App/Application.h"
#include "Example/VulkanExampleApp.h"

int main(int argc, char** argv)
{
    South::Logger::Init();

    STH_INFO("Application Start");

    auto* App = new South::VulkanTutorial::Application;

    App->Run();

    delete App;

    STH_INFO("Application End");

    South::Logger::DeInit();


    return 0;
}