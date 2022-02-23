#include"FactoryNotEditedSimulation.h"
#include"Simulation.h"
#include"NotEditedSimulation.h"

Simulation FactoryNotEditedSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt) 
{
	return NotEditedSimulation(size, diffiusion, viscosity, dt);
}