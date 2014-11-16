//
//  topologies.cpp
//  tman
//
//  Created by Yang LIU on 10/23/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include <math.h>
#include "ring_topo.h"

Ring_Topo::Ring_Topo(size_t topo_size):Topo(topo_size) {
    _unreachable = (get_topo_size()+2)/2;
    topo_name = std::string("Ring");
}

Ring_Topo::~Ring_Topo() {}

size_t Ring_Topo::distant(size_t id1, size_t id2) const{
	if(!contains(id1) || !contains(id2)) { return _unreachable; }
    if (id1 < id2) {
        size_t t = id1;
        id1 = id2;
        id2 = t;
    }
    size_t dis = id1-id2;
    return dis>=_unreachable ? get_topo_size()-id1+id2 : dis;
}

std::vector<double> Ring_Topo::node_qth(size_t tid) const{
    std::vector<double> qth;
    if(contains(tid)) {
        double angle = 2*PI*((double)tid/get_topo_size());
        _DEBUG(angle);
        qth.push_back(sin(angle));
        qth.push_back(cos(angle));
    }
    return qth;
}

std::string Ring_Topo::get_topo_name() const{
    return std::string(topo_name);
}

