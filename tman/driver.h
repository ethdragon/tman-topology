//
//  driver.h
//  tman
//
//  Created by Yang LIU on 11/2/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#ifndef __tman__driver__
#define __tman__driver__

#include <Python.h>
#include <stdio.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "ring_topo.h"
#include "d_topo.h"

using boost::property_tree::ptree;
using boost::property_tree::write_json;

class Drive {
public:
    Drive (size_t neighb=30, double cut_size=0.3);
    virtual ~Drive ();
    
    void set_topo(char t, size_t t_size=1000);
    void set_node_size(size_t node_size=800);
    void set_neighb_size(size_t neighb=30);
    void set_topo_size(size_t t_size=1000);
    void set_logger (int log_interveral=1);
    
    void init_stage();
    void evolution (int times=1);
    void terminate_stage(bool t);
    void write2json(const char* filename=0, bool concise=false) const;
    
private:
    Topo *topo;
    size_t neighb_size;
    size_t node_size;
    size_t topo_size;
    char _t;
    int interval;
    double cut_size;
    std::list<Node *> node_list;
    
    void update_prespective();
    void build_topo(char, size_t);
    void write_file (int iteration, const char* appendix=0);
    
    ptree node2ptree(Node const*, bool concise=false) const;
    ptree generate_ptree();
};

void update_prespective(std::list<Node *> &nlist);
void evolution (std::list<Node *> &nlist, Topo *);
void log_neighbours (std::list<Node> nlist, int iteration);

#endif /* defined(__tman__driver__) */
