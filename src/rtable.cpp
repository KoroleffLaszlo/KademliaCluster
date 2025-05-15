#include "../include/rtable.hpp"

#include <fstream>
#include <cstring>
#include <string>
#include <bitset>
#include <map>
#include <iostream>
#include <cerrno>

const char* RouteTable::_get_machine_id()
{
    std::ifstream file("/etc/machine-id");
    std::string _id;

    if (file.is_open()) {
        std::getline(file, _id);
        file.close();

    }else {
        throw std::runtime_error(std::string("[ERROR] '/etc/machine-id' file doesn't exist: ") 
                                + std::string(strerror(errno)));
    }

    return _id.c_str();
}

/*
returns the index of first instance of up bit in distance between *this* id and
destination id
*/
int RouteTable::_msb_search(const std::bitset<ID_SIZE>& dest_id)
{
    std::bitset<ID_SIZE> distance = dest_id ^ this->self_id;

    if (distance == 0) {return -1;} // indicates its our own id

    for(auto i = 0; i < ID_SIZE; i++) {
        if (dest_id[i]) { return ID_SIZE - 1 - i;}
    }

    return -1; 
}

/*
TODO:   self id set by device machine-id + port number 
        uses SHA to encode into bytes
*/
void RouteTable::set_self_id()
{
    const char* machine_id = _get_machine_id();
    std::cout << "Device machine-id: "<< machine_id << '\n';
}

const std::bitset<ID_SIZE>& RouteTable::get_self_id() const
{
    return this->self_id;
}

/*
TODO:   logic for store node 
        check for K value size 

*/
void RouteTable::store_node(const Node& _node)
{
    return;
}

RouteTable::Node* RouteTable::get_node(const std::bitset<ID_SIZE> node_id)
{

    int msb_index = _msb_search(node_id);
    if (msb_index < 0) {return nullptr;}

    std::vector<Node>& cluster = global_cluster[msb_index];

    // TODO: if cluster size >= K size -> then LRU 

    for (auto& _node : cluster) {
        if (_node.id == node_id) {return &_node;}
    }

    return nullptr;
}

