// AV_OOP_KP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include "app\Application.hpp"

int main(int argc, char* argv[])
{
    av::Application app(argc, argv);
    return app.Run();;
}