#pragma once
#include "Simulation.h"

class Factory
{
public :
	virtual Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt) = 0;
	virtual ~Factory() {};
};
