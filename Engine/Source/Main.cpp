#include "sthpch.h"

#include "Core/App/Application.h"
#include "Example/VulkanExampleApp.h"

int main(int argc, char** argv)
{
    South::Logger::Init();

    STH_INFO("Application Start");

    South::VulkanTutorial::Application App;
    App.Run();

    // auto* App = new South::Application;
    //
    // App->Run();
    //
    // delete App;

    STH_INFO("Application End");

    South::Logger::DeInit();


    return 0;
}