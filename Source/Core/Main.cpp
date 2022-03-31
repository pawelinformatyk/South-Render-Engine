#include "Core/Application.h"

#include <iostream>

int main(int argc, char** argv)
{
    South::Application App(argc, argv);

    try
    {
        App.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}