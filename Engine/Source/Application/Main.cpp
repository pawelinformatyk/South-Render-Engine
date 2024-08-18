#include "Application/App.h"

// #TODO: Make App, Editor, Core modules with Public/Private dirs. What about sthpch?

int main(int, char**)
{
    auto* App = new South::SApplication;

    App->Run();

    delete App;

    return 0;
}