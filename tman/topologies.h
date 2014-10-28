//
//  topologies.h
//  tman
//
//  Created by Yang LIU on 10/23/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#ifndef __tman__topologies__
#define __tman__topologies__

#include <stdio.h>
#include "tman.h"

#define PI 3.14159265

class Ring_Topo : public Topo {
public:
    Ring_Topo();
    Ring_Topo(size_t topo_size);
    virtual ~Ring_Topo();
    
    virtual size_t distant(size_t, size_t);
    virtual std::vector<double> node_qth (size_t);
};

class D_Topo : public Topo {
public:
    D_Topo();
    D_Topo(size_t topo_size, double cut_rate);
    virtual ~D_Topo();
    
    virtual size_t distant(size_t, size_t);
    virtual std::vector<double> node_qth (size_t);
    
protected:
    double cut_rate;
};

#endif /* defined(__tman__topologies__) */
