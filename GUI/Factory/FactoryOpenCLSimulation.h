#pragma once
#include"Factory.h"
#include"pch.h"

class FactoryOpenCLSimulation : public Factory
{
public:
	Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt);
	~FactoryOpenCLSimulation() {};
};