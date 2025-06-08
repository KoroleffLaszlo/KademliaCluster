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

#define CHUNK_SIZE 4

/*
converts strings and integers to unsigned long variables
Used later to convert to hex and into bitset for future self_id processing 
*/
template<typename T>
std::enable_if_t<std::is_integral<T>::value, unsigned long>
_toul(const T& val)
{
    return static_cast<uninsigned_long>(val);
}

template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, unsigned long>
_toul(const T& val)
{
    return std::stoul(val, nullptr, 16);
}

/*
string or int conversion to bitset used in parent function 
return:
    int : bitshift value for later additions to _id 
*/
template<typename T>
std::enable_if_t<std::is_integral<T>::value, int>
_toid(const T& val, const std::bitset<ID_SIZE>& _id, size_t pos = npos)
{
    return;
}

template<typename T>
std::enable_if_t<std::is_same_v<T, std::string>, int>
_toid(const T& val, const std::bitset<ID_SIZE>& _id, size_t pos = npos)
{   
    std::string _val = val;
    size_t chunk(_chunking(_val));
    while(chunk)
    {
        _val.erase(_val.length() - chunk);
        auto ul(_toul(_val.substr(section)));
        chunk = _chunking(_val);
    }
    return;
}

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

int RouteTable::_chunking(const std::string& val)
{
    return val.length() > CHUNK_SIZE ? CHUNK_SIZE : val.length();
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

// PUBLIC FUNCTIONS

void RouteTable::set_self(const int& port, const std::string& ip)
{
    this->self_port = port_num;
    this->self_ip = ip;
    std::string machine_id = _get_machine_id();

    std::cout << "Device machine-id: "<< "|" << machine_id << "|" << "\n";
    std::bitset<ID_SIZE> _id{};

    // remove non-hexable values in machine_id to avoid conversion errors
    machine_id.erase(remove_if(machine_id.begin(), machine_id.end(), 
                    [](char c) {return !isxdigit(static_cast<unsigned char>(c));}), // lambda : return true if NOT hex value
                machine_id.end());

    std::cout<< "Modified machine-id: "<< "|" << machine_id << "|" << "\n";


    int port_len = port > 0 ? log10(port) + 1 : 1; //number of digits in port used for chunking

    this->self_id = _id;
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

