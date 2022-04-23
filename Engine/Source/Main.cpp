#include "sthpch.h"

#include "Core/Application.h"

int main(int argc, char** argv)
{
    auto& App = South::Application::Get();

    App.Init();
    App.Run();
    App.DeInit();

    return 0;
}