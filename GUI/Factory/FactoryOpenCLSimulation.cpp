#include "pch.h"
#include "Simulation.h"
#include "FactoryOpenCLSimulation.h"
#include "OpenCLSimulation.h"

Simulation* FactoryOpenCLSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new OpenCLSimulation(size, diffiusion, viscosity, dt);
}