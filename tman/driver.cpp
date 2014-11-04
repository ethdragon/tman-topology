//
//  driver.cpp
//  tman
//
//  Created by Yang LIU on 11/2/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include "driver.h"

Drive::Drive (size_t neighb, double cut_size) {
    this->neighb = neighb;
    this->cut_size = cut_size;
    this->level = NODE;
}

Drive::~Drive () {
    if (topo) {
        delete topo;
    }
}

void Drive::set_topo(char t, size_t t_size) {
    switch (t) {
        case 'r':
        case 'R':
            topo = new Ring_Topo(t_size);
            break;
            
        case 'd':
        case 'D':
            topo = new D_Topo(t_size, cut_size);
            
        default:
            topo = 0;
            printf("No such Topology\n");
            exit(-1);
            break;
    }
    if (!topo) {
        printf("Allocate topology failed\n");
        exit(-1);
    }
}

void Drive::set_node_size(size_t node_size) {
    this->node_size = node_size;
}

void Drive::set_neighb_size (size_t neighb) {
    this->neighb = neighb;
}

void Drive::init_stage() {
    Node *node = 0;
    while (topo->get_member_size() < node_size) {
        node = new Node ();
        if (!node) {
            terminate_stage(true);
            printf("Allocate Node failed\n");
            exit(-1);
        }
        if (node->join(topo)) {
            node_list.push_back(node);
        } else {
            delete node;
        }
    }
    update_prespective();
}

void Drive::evolution(int times) {
    int i;
    for (i=0; i<times; i++) {
        std::list<Node *>::iterator it;
        Node *node = 0;
        for (it=node_list.begin(); it!=node_list.end(); it++) {
            node = *it;
            size_t ntid = node->get_rand_neighbour();
            if (!ntid) { continue; }
            Node *neighb = topo->get_node(ntid);
            if (neighb) {
                std::vector<size_t> neighb_prespective, node_prespective;
                neighb_prespective = neighb->get_prespective();
                node_prespective = node->get_prespective();
                
                neighb->update_prespective(node_prespective);
                node->update_prespective(neighb_prespective);
            }
        }
    }
}


void Drive::terminate_stage(bool t) {
    std::list<Node *>::iterator it;
    for (it=node_list.begin(); it!=node_list.end(); it++) {
        delete *it;
    }
    if (t) {
        delete topo;
    }
}

void Drive::update_prespective() {
    Node *node = 0;
    std::list<Node *>::iterator it;
    for (it=node_list.begin(); it!=node_list.end(); it++) {
        node = *it;
        node->update_prespective();
    }
}

void log_neighbours (std::list<Node> nlist, int iteration=0) {
    std::list<Node>::iterator it;
    std::vector<size_t>::iterator iter;
    char fname[50];
    for (it=nlist.begin(); it!=nlist.end(); it++) {
        FILE *fp = 0;
        sprintf(fname, "%zu_%zu_%d_%s.txt",
                it->get_tid(),
                it->get_id(),
                iteration,
                (it->get_topo_name()).c_str()
                );
        fp = fopen(fname, "a");
        fprintf(fp, "%zu\n", it->get_curr_neighb_size());
        std::vector<size_t> neighb = it->get_prespective();
        for (iter=neighb.begin(); iter!=neighb.end(); iter++) {
            fprintf(fp, "%zu\t%zu\n", it->get_tid(), *iter);
        }
        fclose(fp);
    }
}
