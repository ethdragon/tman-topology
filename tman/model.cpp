//
//  topology.cpp
//  tman
//
//  Created by Yang LIU on 10/16/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include <Python.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <unordered_set>
#include "model.h"

Node::Node(size_t neighbour_size) {
    //srand((unsigned int)time(NULL));
    this->neighbour_size = neighbour_size;
    this->id = rand();
    this->topo = 0;
    this->tid = 0;
    neighbours = {};
}

Node::~Node() {
    if (!topo) {
        leave();
    }
    neighbours.clear();
}

std::string Node::get_topo_name() {
    if (!topo) {
        std::string str;
        return str;
    }
    return topo->get_topo_name();
}

size_t Node::get_id() const{
    return id;
}

size_t Node::get_tid() const{
    return tid;
}

size_t Node::get_curr_neighb_size() const{
	return neighbours.size();
}

size_t Node::get_max_neighb_size() const{
    return neighbour_size;
}

bool Node::join(Topo *t) {
    if (!t) { return false; }
    tid = t->join(this);
    if(!tid) { return false; }
    this->topo = t;
    init_neighbours();
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
    std::unordered_set<size_t> hset;
    std::vector<Pair>::iterator pit;
    hset.insert(tid);
    for (pit=neighbours.begin(); pit!=neighbours.end(); pit++) {
        hset.insert(pit->first);
    }
    
    std::vector<size_t>::iterator it;
    for (it=nlist.begin(); it!=nlist.end(); it++) {
        if(hset.find(*it)==hset.end()) {
            Pair neighbour;
            neighbour.first = *it;
            neighbour.second = topo->distant(tid, *it);
            neighbours.push_back(neighbour);
        }
    }
    
    /*
    sort(neighbours.begin(), neighbours.end(), cmp);
    if(neighbours.size() > neighbour_size) {
	    neighbours.resize(neighbour_size);
    }*/
    size_t neighbour_effictive = update_prespective();
    
    return neighbour_effictive;
}

// clear the unreachable neighbours
size_t Node::update_prespective() {
    if (!topo || !neighbours.size()) { return 0; }

    size_t i=0, j=neighbours.size()-1, unreachable=topo->unreachable();
    size_t ntid = 0, distance = unreachable;
    while (i<=j) {
        ntid = neighbours[i].first;
        distance = topo->distant(tid, ntid);
        neighbours[i].second = distance;
        if (distance >= unreachable) {  // careful unsigned variable won't go negetive
            Pair t = neighbours[i];
            neighbours[i] = neighbours[j];
            neighbours[j] = t;
            if (j) { j--; }
            else { break; }
            continue;
        }
        i++;
    }
    size_t size = j+1 > neighbour_size ? neighbour_size : j+1;
    
    sort(neighbours.begin(), neighbours.end(), cmp);
    neighbours.resize(size);
    
    
    /*
    size_t cnt = neighbours.size();
    std::vector<Pair>::reverse_iterator rit;
    for (rit=neighbours.rbegin(); rit!=neighbours.rend(); rit++) {
        if (rit->second < unreachable) { break; }
        cnt--;
    }
    neighbours.resize(cnt);
    return cnt;*/
    return neighbours.size();
}

// return the list of tid of current neighbours
std::vector<size_t> Node::get_prespective() const{
    std::vector<size_t> nlist = {};
    if (!topo) { return nlist; }
    
    nlist.get_allocator().allocate(neighbours.size());
    
    std::vector<Pair>::const_iterator it;
    for (it=neighbours.begin(); it!=neighbours.end(); it++) {
        nlist.push_back(it->first);
    }
    //nlist.assign(neighbours.begin(), neighbours.end());
    //nlist.insert(neighbours.begin(), neighbours.begin(), neighbours.end());
    return nlist;
}

size_t Node::get_rand_neighbour() const{
    if (!topo || !get_curr_neighb_size()) { return 0; }
    return neighbours[rand()%get_curr_neighb_size()].first;
}

void Node::init_neighbours() {
    if (!topo) { return; }
    size_t i, ntid=0;
    // Make sure the random picked neighbour doesn't repeat
    std::unordered_set<size_t> set;
    for (i=0; i<neighbour_size; i++) {
        ntid = rand()%topo->get_topo_size()+1;
        if (ntid == tid) { continue; }
        set.insert(ntid);
    }
    
    // Choose random nodes as neighbours
    neighbours.get_allocator().allocate(neighbour_size+neighbour_size);
    std::unordered_set<size_t>::iterator it;
    for (it=set.begin(); it!=set.end(); it++) {
        if (*it != tid) {
            Pair p(*it, topo->distant(tid, *it));
            neighbours.push_back(p);
        }
    }
    
    // sort(neighbours.begin(), neighbours.end(), cmp);
    // update distance and clear unreachable neighbours
    // update_prespective();
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

bool Topo::contains(size_t tid) const{
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

size_t Topo::get_member_size() const{
    return topo_size - tid_pool.size();
}

size_t Topo::get_topo_size() const{
    return topo_size;
}

size_t Topo::unreachable() const{
    return _unreachable;
}

Node *Topo::get_node(size_t id) const{
	std::unordered_map<size_t, Node *>::const_iterator it =
        map.find(id);
    if (it == map.end()) {
    	return 0;
    }
    return it->second;
}

