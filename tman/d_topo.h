//
//  d_topo.h
//  tman
//
//  Created by Yang LIU on 11/16/14.
//  Copyright (c) 2014 Yang Liu. All rights reserved.
//

#ifndef __tman__d_topo__
#define __tman__d_topo__

#include <stdio.h>
#include "model.h"

#define PI 3.14159265

class D_Topo : public Topo {
public:
    D_Topo(size_t topo_size=1000, double cut_rate=0.3);
    virtual ~D_Topo();
    
    virtual size_t distant(size_t, size_t) const;
    virtual std::vector<double> node_qth (size_t) const;
    virtual std::string get_topo_name() const;
    
protected:
    double cut_rate;
};


#endif /* defined(__tman__d_topo__) */
