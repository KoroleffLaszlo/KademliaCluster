#include "../include/rtable.hpp"

#include <fstream>
#include <cstring>
#include <string>
#include <bitset>
#include <map>
#include <iostream>
#include <cerrno>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <type_traits>

std::string RouteTable::_get_machine_id()
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
    return _id;
}

/*
returns the index of first instance of up bit in distance between *this* id and
destination id
*/
int RouteTable::_msb_search(const std::bitset<ID_SIZE>& dest_id)
{
    std::bitset<ID_SIZE> distance = dest_id ^ this->self_id;

    if (distance == 0) {return -1;} // indicates its our own id

    for (auto i = 0; i < ID_SIZE; i++) { 
        if (dest_id[i]) {return ID_SIZE - 1 - i;} // returns the first instance of the up bit 
    }

    return -1;
}

// avoiding overhead cost of using regex
std::vector<std::string> RouteTable::_ip_handle(const std::string& ip)
{   
    std::vector<std::string> ip_groups{};
    std::string group = "";
    
    for (auto& c : ip) {
        if (std::ispunct(static_cast<unsigned char>(c))) {
            ip_groups.push_back(group);
            group = "";
        } else{group += c;}
    }
    if (!group.empty()) {
        ip_groups.push_back(group);
    }
    return ip_groups;
}

void RouteTable::set_self(const int& port, const std::string& ip)
{
    this->self_port = port;
    this->self_ip = ip;
    std::string machine_id = _get_machine_id();

    std::cout << "Device machine-id: "<< "|" << machine_id << "|" << "\n";
    std::bitset<ID_SIZE> _id{};

    
    // Remove non-hexable values in machine_id to avoid conversion errors
    machine_id.erase(remove_if(machine_id.begin(), machine_id.end(), 
                    [](char c) {return !isxdigit(static_cast<unsigned char>(c));}), // lambda : return true if NOT hex value
                machine_id.end());
    
    // Handling machine_id 
    int pos(_toid(machine_id, _id));
    
    // Handling port
    pos = _toid(port, _id, pos);

    // Handling ip
    std::vector<std::string> ip_groups(_ip_handle(ip));
    for (auto& i : ip_groups) {
        std::cout<<i<<"\n";
        pos = _toid(i, _id, pos);
    }

    std::cout<<"ID: "<<_id<<"\n";
    this->self_id = _id;
    return;
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

