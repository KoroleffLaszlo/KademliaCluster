#include "../include/rtable.hpp"

#include <iostream>

RouteTable table_handler;

int main(int argc, char* argv[])
{
    table_handler.set_self();

    return EXIT_SUCCESS;
}