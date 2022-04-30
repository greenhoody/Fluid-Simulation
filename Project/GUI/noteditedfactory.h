#pragma once

#ifndef NOTEDITEDFACTORY_H
#define NOTEDITEDFACTORY_H

#include "factory.h"

class NotEditedFactory : public Factory
{
public:
    Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt) override;
    //NotEditedFactory(){};
};
#endif // NOTEDITEDFACTORY_H
