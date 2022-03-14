#include "FactoryEditedSimulation.h"
#include "EditedSimulation.h"
#include "Simulation.h"
#include "pch.h"

Simulation* FactoryEditedSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new EditedSimulation(size, diffiusion, viscosity, dt);
}