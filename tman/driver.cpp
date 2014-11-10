//
//  driver.cpp
//  tman
//
//  Created by Yang LIU on 11/2/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include "driver.h"

Drive::Drive (size_t neighb_size, double cut_size) {
    srand((unsigned int)time(NULL));
    this->neighb_size = neighb_size;
    this->cut_size = cut_size;
    this->interval = 1;
    this->_t = 'r';
}

Drive::~Drive () {
    if (topo) {
        delete topo;
    }
}

void Drive::set_topo(char t, size_t t_size) {
    this->_t = t;
    this->topo_size = t_size;
}

void Drive::set_topo_size(size_t t_size) {
    this->topo_size = t_size;
}

void Drive::build_topo(char t, size_t t_size) {
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

void Drive::set_neighb_size (size_t neighb_size) {
    this->neighb_size = neighb_size;
}

void Drive::init_stage() {
    build_topo(_t, topo_size);
    Node *node = 0;
    while (topo->get_member_size() < node_size) {
        node = new Node (neighb_size);
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
    write_file(0);
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
        if (!(i%interval)) {
            write_file(i+1);
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

void Drive::set_logger(int log_interval) {
    interval = log_interval;
}

void Drive::write_file(int iteration, const char * appendix) {
    std::list<Node *>::iterator it;
    FILE *fp = 0;
    char fname[100];
    if (appendix) {
        sprintf(fname, "%s_t%zu_n%zu_k%zu_i%d_%s.txt",
                (topo->get_topo_name()).c_str(),
                topo->get_topo_size(),
                node_size,
                neighb_size,
                iteration,
                appendix);
    } else {
        sprintf(fname, "%s_t%zu_n%zu_k%zu_i%d.txt",
                (topo->get_topo_name()).c_str(),
                topo->get_topo_size(),
                node_size,
                neighb_size,
                iteration);
    }
    fp = fopen(fname, "w");
    for (it=node_list.begin(); it!=node_list.end(); it++) {
        std::vector<size_t>::iterator it2;
        std::vector<size_t> nei = (*it)->get_prespective();
        size_t tid = (*it)->get_tid();
        for (it2=nei.begin(); it2!=nei.end(); it2++) {
            fprintf(fp, "%zu %zu %zu\n",
                    tid,
                    *it2,
                    topo->distant(tid, *it2));
        }
    }
    fclose(fp);
}

void Drive::write2json() {
    Node *n = node_list.front();
    ptree pt = node2ptree(n);
    
    //boost::property_tree::json_parser::write_json("node.json", pt);
    
    /**/
    std::ostringstream buf;
    write_json (buf, pt, false);
    std::string json = buf.str();
    std::cout<<json<<'\n';
}

ptree Drive::node2ptree(Node *n) {
    ptree pt, pt_neighbs;
    if (!n) {
        return pt;
    }
    std::vector<double> xoy = topo->node_qth(n->get_tid());
    pt.put("id", n->get_id());
    pt.put("tid", n->get_tid());
    pt.put("x", xoy[0]);
    pt.put("y", xoy[1]);
    std::vector<size_t> neighbours = n->get_prespective();
    std::vector<size_t>::iterator it;
    
    for (it=neighbours.begin(); it!=neighbours.end(); it++) {
        _DEBUG("in for loop");
        xoy = topo->node_qth(*it);
        ptree pt_neighb;
        pt_neighb.put("tid", *it);
        pt_neighb.put("x", xoy[0]);
        pt_neighb.put("y", xoy[1]);
        pt_neighbs.push_back(std::make_pair("", pt_neighb));
    }
    
    pt.put_child("neighbours", pt_neighbs);
    return pt;
}


