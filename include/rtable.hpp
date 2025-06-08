#ifndef TABLE
#define TABLE

#include <iostream>
#include <bitset>
#include <string>
#include <vector>

#define ID_SIZE 160     // bit size of id 
#define K_SIZE 8        // number of nodes allowed per bucket 
class RouteTable {
public:
    struct Node {
        std::bitset<ID_SIZE> id{};
        std::string ip_addr{}; //char * - more memory efficient - won't be changed 
        int port{-1};
    };

    void set_self();
    const std::bitset<ID_SIZE>& get_self_id() const;


    void store_node(const Node&);
    Node* get_node(const std::bitset<ID_SIZE>);


    /*
    debug functions used for local use only
    */
    //void debug_print_node_clusters();

private:

    std::bitset<ID_SIZE> self_id{};
    int self_port{-1};
    std::string self_ip{};

    /*
    TODO: add additional clusters for local and region
    */
    // std::vector<Node> local_cluster[ID_SIZE];
    // std::vecotr<Node> region_cluster[ID_SIZE];
    std::vector<Node> global_cluster[ID_SIZE];

    int _msb_search(const std::bitset<ID_SIZE>&);

        /*
    used strictly for set_self_id() 
    */
    std::string _get_machine_id();
};


#endif