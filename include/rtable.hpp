#ifndef TABLE
#define TABLE

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <unordered_map>

#define ID_SIZE 160     // bit size of id 
#define K_SIZE 20
#define CHUNK_SIZE 4

class RouteTable {
public:
    struct Node {
        std::bitset<ID_SIZE> id{};
        std::string ip_addr{};
        std::size_t port{};
    };

    void set_self(const int&, const std::string&);
    const std::bitset<ID_SIZE>& get_self_id() const;


    void store_node(const Node&);
    Node* get_node(const std::bitset<ID_SIZE>);


    /*
    debug functions used for local use only
    */
    //void debug_print_node_clusters();

private:

    struct Bucket {
        std::unordered_map<std::size_t, Node> cluster;
    };

    std::bitset<ID_SIZE> self_id{};
    std::size_t self_port{};
    std::string self_ip{};


    std::vector<Bucket> route_table{ID_SIZE};
    
    std::size_t _msb_search(const std::bitset<ID_SIZE>&);

    /*
    used strictly for set_self_id() 
    */
    std::string _get_machine_id();

    std::size_t _chunking(const std::string& val)
    {
        return val.length() > CHUNK_SIZE ? CHUNK_SIZE : val.length();
    }

    std::vector<std::string> _ip_handle(const std::string&);

    /*
    converts strings and integers to unsigned long variables
    Used later to convert to hex and into bitset for future self_id processing 
    */
    template<typename T>
    std::enable_if_t<std::is_integral<T>::value, unsigned long>
    _toul(const T& val)
    {
        return static_cast<unsigned long>(val);
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
    _toid(const T& val, std::bitset<ID_SIZE>& _id, size_t pos = 0)
    {
        std::string _val(std::to_string(val));
        size_t chunk(_chunking(_val)); // at start be CHUNK_SIZE (4)
        while(chunk)
        {
            auto ul(_toul(_val.substr(_val.length() - chunk)));
            _val.erase(_val.length() - chunk);
            chunk = _chunking(_val);
            std::bitset<16> b(ul);
            _id |= (std::bitset<ID_SIZE>(b.to_ulong()) << pos);
            pos += 16;
        }
        return pos; 
    }

    template<typename T>
    std::enable_if_t<std::is_same_v<T, std::string>, int>
    _toid(const T& val, std::bitset<ID_SIZE>& _id, size_t pos = 0)
    {
        std::string _val = val;
        size_t chunk(_chunking(_val));
        while(chunk)
        {
            auto ul(_toul(_val.substr(_val.length() - chunk)));
            _val.erase(_val.length() - chunk);
            chunk = _chunking(_val);
            std::bitset<16> b(ul);
            _id |= (std::bitset<ID_SIZE>(b.to_ulong()) << pos);
            pos += 16;
        }
        return pos;
    }
};
#endif