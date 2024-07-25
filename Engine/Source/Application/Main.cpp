#include "Application/VulkanExampleApp.h"

// #TODO: Make App, Editor, Core modules with Public/Private dirs. What about sthpch?

int main(int, char**)
{
    auto* App = new South::Application;

    App->Run();

    delete App;

    return 0;
}