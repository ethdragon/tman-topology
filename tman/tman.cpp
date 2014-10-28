//
//  topology.cpp
//  tman
//
//  Created by Yang LIU on 10/16/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <unordered_set>
#include "tman.h"

Node::Node() {
    srand((unsigned int)time(NULL));
    this->neighbour_size = 30;
    this->id = rand();
    this->topo = 0;
    this->tid = 0;
    neighbours = {};
}

Node::Node(size_t id, size_t neighbour_size) {
    srand((unsigned int)time(NULL));
    this->id = id;
    this->tid = 0;
    this->neighbour_size = neighbour_size;
    this->topo = 0;
    neighbours = {};
}

Node::~Node() {
    if (!topo) {
        leave();
    }
    neighbours.clear();
}

size_t Node::get_id() {
    return id;
}

size_t Node::get_tid() {
    return tid;
}

size_t Node::get_curr_neighb_size() {
	return neighbours.size();
}

size_t Node::get_max_neighb_size() {
    return neighbour_size;
}

bool Node::join(Topo *t) {
    if (!t) { return false; }
    tid = t->join(this);
    if(!tid) { return false; }
    this->topo = t;
    //init_neighbours();
    return true;
}

bool Node::leave() {
    if (!topo->leave(tid)) {
        return false;
    }
    neighbours.clear();
    tid = 0;
    return true;
}

size_t Node::update_prespective(std::vector<size_t> nlist) {
    if (!topo) { return 0; }
    //neighbours.assign(nlist.begin(), nlist.end());
    
    // allocate memory
    neighbours.get_allocator().allocate(nlist.size()+neighbours.size());
    std::vector<size_t>::iterator it;
    for (it=nlist.begin(); it!=nlist.end(); it++) {
        std::pair<size_t, size_t> neighbour;
        neighbour.first = *it;
        neighbour.second = topo->distant(tid, *it);
    }
    
    sort(neighbours.begin(), neighbours.end(), cmp);
    if(neighbours.size() > neighbour_size) {
	    neighbours.resize(neighbour_size);
    }
    return neighbour_size;
}

// clear the unreachable neighbours
size_t Node::update_prespective() {
    if (!topo) { return 0; }
    
    sort(neighbours.begin(), neighbours.end(), cmp);
    size_t cnt = neighbours.size();
    size_t unreachable = topo->unreachable();
    std::vector<Pair>::reverse_iterator it;
    for (it=neighbours.rbegin(); it!=neighbours.rend(); it++) {
        if (it->second < unreachable) { break; }
        cnt--;
    }
    neighbours.resize(cnt);
    return cnt;
}

// return the list of tid of current neighbours
std::vector<size_t> Node::get_prespective() {
    std::vector<size_t> nlist = {};
    if (!topo) { return nlist; }
    
    nlist.get_allocator().allocate(neighbours.size()); //for optimization
    std::vector<std::pair<size_t, size_t>>::iterator it;
    for (it=neighbours.begin(); it!=neighbours.end(); it++) {
        nlist.push_back(it->first);
    }
    //nlist.assign(neighbours.begin(), neighbours.end());
    //nlist.insert(neighbours.begin(), neighbours.begin(), neighbours.end());
    return nlist;
}

size_t Node::get_rand_neighbour() {
    if (!topo || !get_curr_neighb_size()) { return 0; }
    return neighbours[rand()%get_curr_neighb_size()].first;
}

void Node::init_neighbours() {
    if (!topo) { return; }
    size_t i;
    std::unordered_set<size_t> set;
    for (i=0; i<neighbour_size; i++) {
        set.insert(rand()%topo->get_topo_size());
    }
    
    std::unordered_set<size_t>::iterator it;
    for (it=set.begin(); it!=set.end(); it++) {
        if (*it != tid) {
            Pair p(*it, topo->distant(tid, *it));
            neighbours.push_back(p);
        }
    }
    sort(neighbours.begin(), neighbours.end(), cmp);
    
    // update distance and clear unreachable neighbours
    update_prespective();
}


Topo::Topo() {
    topo_size = 1000;
    init();
}

Topo::Topo(size_t topo_size) {
	this->topo_size = topo_size;
    init();
}

Topo::~Topo() { }

void Topo::init() {
    size_t i = 1;
    // Looping with size_t may have performance penalty, however only execute once
    // http://stackoverflow.com/questions/10596350/cast-performance-from-size-t-to-double
    while (i<=topo_size) {
        tid_pool.push_back(i++);
    }
    _unreachable = std::numeric_limits<size_t>::max();
    map = {};
}

bool Topo::contains(size_t tid) {
    std::unordered_map<size_t, Node *>::const_iterator it =
        map.find(tid);
    return it != map.end();
}

// use the node id as the topology id, should make a mapping 
// between the node id and the topology id
size_t Topo::join(Node *n) {
    if (!n || contains(n->get_tid())) { return 0; }
    if (tid_pool.size()<=0) { return 0; }
    
    size_t tid = tid_pool.front();
    tid_pool.pop_front();
    //map[tid] = n;
    map.insert({tid, n});
    return tid;
}

bool Topo::leave(size_t tid) {
    if (!contains(tid)) { return false; }
    map.erase(tid);
    tid_pool.push_back(tid);
    return true;
}

size_t Topo::get_member_size() {
    return topo_size - tid_pool.size();
}

size_t Topo::get_topo_size() {
    return topo_size;
}

size_t Topo::unreachable() {
    return _unreachable;
}

Node *Topo::get_node(size_t id) {
	std::unordered_map<size_t, Node *>::const_iterator it =
        map.find(id);
    if (it == map.end()) {
    	return 0;
    }
    return it->second;
}
