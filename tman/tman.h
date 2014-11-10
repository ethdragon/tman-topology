//
//  tman.h
//  tman
//
//  Created by Yang LIU on 10/16/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#ifndef tman_tman_h
#define tman_tman_h

#define _MY_DEBUG_FLG 0

#define _DEBUG(x) do { \
if (_MY_DEBUG_FLG) { \
    auto _timestamp = std::time(0); \
    auto _tm = *std::localtime(&_timestamp); \
    std::cout<<std::put_time(&_tm, "[%Y-%m-%d %H:%M:%S]")<<"|DEBUG| "<<x<<'\n';\
    }\
} while (0)

#include <stdarg.h>
#include <list>
#include <ctime>
#include <vector>
#include <iomanip>
#include <string>
#include <iostream>
#include <unordered_map>
#include <initializer_list>

class Node;
class Topo;

typedef std::pair<size_t, size_t> Pair;

//http://www.cplusplus.com/reference/algorithm/sort/?kw=sort
struct my_comparator {
    bool operator ()(Pair n1, Pair n2) {
        return n1.second==n2.second ?
                n1.first<n2.first :
                n1.second<n2.second;
    }
};
typedef struct my_comparator my_comparator;

class Node{
public:
    Node(size_t size=30);                       // init the node with 100 neighbours, random id
    virtual ~Node();
    virtual std::string get_topo_name ();
    
    void init_neighbours ();
    size_t get_id() const;                            // return the id of the node
    size_t get_tid() const;
    size_t get_max_neighb_size() const;
    size_t get_curr_neighb_size() const;
    bool join (Topo *);                         // join the topology, return true if successed
    bool leave ();                              // leave the topology, return ture if no errors
    size_t update_prespective(std::vector<size_t>);  // update local prespective, return the size
                                                     // of the neighbour after updates
    size_t update_prespective();
    size_t get_rand_neighbour() const;
    std::vector<size_t> get_prespective () const;     // return the current local prespective
    
protected:
    size_t id, neighbour_size, tid;
    Topo *topo;                                 // joined topology
    std::vector<Pair> neighbours;         // list of neighbour ids
    
private:
    my_comparator cmp;
};

class Topo{
public:
    Topo(size_t topo_size=1000);
    virtual ~Topo();
    
    virtual size_t distant(size_t, size_t) const = 0;     // return the distance of the two nodes
    virtual std::vector<double> node_qth (size_t) const = 0;  // return the coordinates/location of the nodes
    virtual std::string get_topo_name () const = 0;
    
    size_t get_member_size() const;
    size_t get_topo_size() const;
    size_t unreachable() const;                     // return the minimum length considered as unreachable
    size_t join (Node *n);                          // on success, return an position id other than 0
                                                    // return 0 if the node fail to join.
    bool leave (size_t);                            // return true of the node leave successfully
    bool contains (size_t) const;
    Node *get_node(size_t id) const;                // return the pointer of the node
    
protected:
    size_t _unreachable;
    std::string topo_name;

private:  //protected:
    size_t topo_size;
    std::list<size_t> tid_pool;
    std::unordered_map<size_t, Node *> map;         // <pos, node>
    
private:
    void init ();
};

#endif
