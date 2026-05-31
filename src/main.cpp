#include "../include/rtable.hpp"

#include <iostream>
#include <string>

RouteTable table_handler;

int main(int argc, char* argv[])
{
    std:size_t port {80};
    std::string ip {"192.92.0.1"};
    // std::string ip = "192";
    table_handler.set_self(port, ip);

    const std::bitset<ID_SIZE> node_id(13467123431313132);

    RouteTable::Node* search_node = table_handler.get_node(node_id);

    return EXIT_SUCCESS;
}