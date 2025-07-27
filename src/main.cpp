#include "../include/rtable.hpp"

#include <iostream>
#include <string>

RouteTable table_handler;

int main(int argc, char* argv[])
{
    int port = 80;
    std::string ip = "192.92.0.1";
    // std::string ip = "192";
    table_handler.set_self(port, ip);

    return EXIT_SUCCESS;
}