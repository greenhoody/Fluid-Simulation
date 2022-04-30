#pragma once
#include"factory.h"

class CudaFactory : public Factory
{
public:
	Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt);
    ~CudaFactory() {};
};
