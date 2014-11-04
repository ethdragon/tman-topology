//
//  driver.h
//  tman
//
//  Created by Yang LIU on 11/2/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#ifndef __tman__driver__
#define __tman__driver__

#include <stdio.h>
#include "topologies.h"

enum log_level {
    NODE,
    TOPOLOGY
};

class Drive {
public:
    Drive (size_t neighb=30, double cut_size=0.3);
    virtual ~Drive ();
    
    void set_topo(char t, size_t t_size=1000);
    void set_node_size(size_t node_size=800);
    void set_neighb_size(size_t neighb=30);
    void set_logger (enum log_level level=NODE,
                     int log_interveral=1);
    
    void init_stage();
    void evolution (int times=1);
    void terminate_stage(bool t);
    
private:
    Topo *topo;
    size_t neighb;
    size_t node_size;
    double cut_size;
    std::list<Node *> node_list;
    enum log_level level;
    
    void update_prespective();
};

void update_prespective(std::list<Node *> &nlist);
void evolution (std::list<Node *> &nlist, Topo *);
void log_neighbours (std::list<Node> nlist, int iteration);

#endif /* defined(__tman__driver__) */
