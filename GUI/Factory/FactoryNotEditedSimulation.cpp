#include"FactoryNotEditedSimulation.h"
#include"Simulation.h"
#include"NotEditedSimulation.h"
#include"pch.h"

Simulation* FactoryNotEditedSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt) 
{
	return new NotEditedSimulation(size, diffiusion, viscosity, dt);
}