//
//  main.cpp
//  tman
//
//  Created by Yang LIU on 9/20/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <list>
#include "topologies.h"

int main(int argc, const char * argv[])
{

    // insert code here...
    Topo *ring, *d;
    ring = new Ring_Topo (10);
    d = new D_Topo(10, 0.2);
    
    // create a list of nodes and join the topology
    // may change to multithreading instead of using a ugly list
    // to keep track of all the nodes
    std::list<Node *> nlist;
    Node *n;
    while (nlist.size() < 10) {
        n = new Node ();
        if (n->join(ring)) {
            nlist.push_back(n);
        }
    }
    
    // topology evolution
    std::list<Node *>::iterator it;
    for (it=nlist.begin(); it!=nlist.end(); it++) {
        n = *it;
        size_t ntid;
        ntid = n->get_rand_neighbour();
        if (!ntid) { continue; }
        Node *neighb = ring->get_node(ntid);
        if (!neighb) {
            std::vector<size_t> prespective = neighb->get_prespective();
            n->update_prespective(prespective);
        }
    }
    
    // delete the nodes
    while (nlist.size()) {
        n = nlist.front();
        nlist.pop_front();
        delete n;
    }
    delete d;
    delete ring;
    printf("Hello World!\n");
    return 0;
}

