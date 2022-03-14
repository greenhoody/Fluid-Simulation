#pragma once
#include"Factory.h"
#include"pch.h"

class FactoryNotEditedSimulation : public Factory
{
public:
	Simulation* CreateSimulation(int size, float diffiusion, float viscosity, float dt);
	~FactoryNotEditedSimulation(){};
};