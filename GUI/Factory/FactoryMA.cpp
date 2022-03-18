#include"FactoryMA.h"
#include"Simulation.h"
#include"SimulationMA.h"
#include"pch.h"

Simulation* FactoryMA::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new SimulationMA(size, diffiusion, viscosity, dt);
}