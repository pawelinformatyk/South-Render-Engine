#include "sthpch.h"

#include "Core/App/Application.h"

int main(int argc, char** argv)
{
    bool bApplicationRunning = true;

    while (bApplicationRunning)
    {
        South::Logger::Init();

        STH_INFO("Application Start");
        STH_INFO("");

        auto* App = new South::Application;

        // #TODO : check App ensure

        App->Init();
        App->Run();
        App->DeInit();

        delete App;

        STH_INFO("");
        STH_INFO("Application Shutdown.");

        South::Logger::DeInit();
    }

    return 0;
}