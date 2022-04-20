#include "sthpch.h"

#include "Core/Application.h"
#include "vulkan/vulkan_core.h"

int main(int argc, char** argv)
{
    auto* App = new South::Application;

    App->Init();
    App->Run();
    App->DeInit();

    delete App;

    return 0;
}