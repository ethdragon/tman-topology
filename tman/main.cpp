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

void evolution (std::list<Node *> &nlist, Topo *);

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
    Node *node;
    while (nlist.size() < 10) {
        node = new Node ();
        if (node->join(ring)) {
            nlist.push_back(node);
        }
    }
    
    evolution(nlist, ring);
    
    // delete the nodes
    while (nlist.size()) {
        node = nlist.front();
        nlist.pop_front();
        delete node;
    }
    delete d;
    delete ring;
    printf("Hello World!\n");
    return 0;
}

void evolution (std::list<Node *> &nlist, Topo *topo) {
    if (!topo) { return; }

    // topology evolution
    std::list<Node *>::iterator it;
    Node *node = 0;
    for (it=nlist.begin(); it!=nlist.end(); it++) {
        node = *it;
        size_t ntid;
        ntid = node->get_rand_neighbour();
        if (!ntid) { continue; }
        Node *neighb = topo->get_node(ntid);
        if (!neighb) {
            std::vector<size_t> neighb_prespective, node_prespective;
            neighb_prespective = neighb->get_prespective();
            node_prespective = node->get_prespective();
            
            neighb->update_prespective(node_prespective);
            node->update_prespective(neighb_prespective);
        }
    }
}

