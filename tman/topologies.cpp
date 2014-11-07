//
//  topologies.cpp
//  tman
//
//  Created by Yang LIU on 10/23/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#include <math.h>
#include "topologies.h"

Ring_Topo::Ring_Topo(size_t topo_size):Topo(topo_size) {
    _unreachable = (get_topo_size()+2)/2;
    topo_name = std::string("Ring");
}

Ring_Topo::~Ring_Topo() {}

size_t Ring_Topo::distant(size_t id1, size_t id2) {
	if(!contains(id1) || !contains(id2)) { return _unreachable; }
    if (id1 < id2) {
        size_t t = id1;
        id1 = id2;
        id2 = t;
    }
    size_t dis = id1-id2;
    return dis>=_unreachable ? get_topo_size()-id1+id2 : dis;
}

std::vector<double> Ring_Topo::node_qth(size_t tid) {
    std::vector<double> qth;
    if(contains(tid)) {
        double angle = 2*PI*((double)tid/get_topo_size());
        _DEBUG(angle);
        qth.push_back(sin(angle));
        qth.push_back(cos(angle));
    }
    return qth;
}

std::string Ring_Topo::get_topo_name() {
    return std::string(topo_name);
}

D_Topo::D_Topo(size_t topo_size, double cut_rate):Topo(topo_size) {
    _unreachable = (get_topo_size()+2)/2;
    this->cut_rate = cut_rate;
    topo_name = std::string("D");
}

D_Topo::~D_Topo() {}

size_t D_Topo::distant(size_t id1, size_t id2) {
    if(!contains(id1) || !contains(id2)) { return _unreachable; }
    if(id1 == id2) { return 0; }
    if (id2 < id1) {  // make sure id1 is smaller
        size_t t = id1;
        id1 = id2;
        id2 = t;
    }
    size_t short_cut = 0.5*cut_rate*get_topo_size();
    if (id1>=1 && id1<=short_cut &&
        id2>=get_topo_size()-short_cut && id2<=get_topo_size()) {
        return 1;
    }
    return id2-id1;
}

std::vector<double> D_Topo::node_qth(size_t tid) {
    std::vector<double> qth;
    if (contains(tid)) {
        // short cut circuit
        size_t short_cut = 0.5*cut_rate*get_topo_size();
        if (tid<=short_cut || tid>=get_topo_size()-short_cut) {
            qth.push_back(0);
            double height = tid <= short_cut ?
                        (double)tid/short_cut :
                        (double)(get_topo_size()-tid)/short_cut;
            if (tid <= short_cut) {
                qth.push_back(height);
            } else {
                qth.push_back(-height);
            }
            return qth;
        }
        // normal circuit
        double vid = double(tid-short_cut);
        double angle = 0.5*PI - PI*(vid/(get_topo_size()-2*short_cut));
        _DEBUG(angle);
        _DEBUG(angle/PI*180);
        qth.push_back(cos(angle));
        qth.push_back(sin(angle));
    }
    return qth;
}

std::string D_Topo::get_topo_name() {
    return std::string(topo_name);
}
