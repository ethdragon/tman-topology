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
#include "driver.h"
#include "topologies.h"

int main(int argc, const char * argv[])
{

    // insert code here...
    Drive driver;
    driver.set_topo('r');
    driver.set_topo_size(1000);
    driver.set_node_size(900);
    driver.set_neighb_size(30);
    
    driver.init_stage();
    driver.evolution(10);
    driver.terminate_stage(0);
    
    printf("Hello World!\n");
    return 0;
}

