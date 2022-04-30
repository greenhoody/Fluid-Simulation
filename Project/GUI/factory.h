#ifndef FACTORY_H
#define FACTORY_H

#include "simulation.h"

class Factory
{
public:
    virtual Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt) = 0;
    virtual ~Factory(){};
};

#endif // FACTORY_H
