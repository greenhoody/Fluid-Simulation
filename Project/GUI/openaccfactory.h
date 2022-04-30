#pragma once

#ifndef OPENACCFACTORY_H
#define OPENACCFACTORY_H

#include "factory.h"

class OpenACCFactory : public Factory
{
public:
    Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt);
    ~OpenACCFactory(){};
};

#endif // OPENACCFACTORY_H
