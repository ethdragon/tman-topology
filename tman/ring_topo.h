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
#include "model.h"

#define PI 3.14159265

class Ring_Topo : public Topo {
public:
    Ring_Topo(size_t topo_size=1000);
    virtual ~Ring_Topo();
    
    virtual size_t distant(size_t, size_t) const;
    virtual std::vector<double> node_qth (size_t) const;
    virtual std::string get_topo_name() const;
};

#endif /* defined(__tman__topologies__) */
