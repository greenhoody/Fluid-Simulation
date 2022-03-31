#include "FactoryCudaSimulation.h"
#include "HostCudaSimulation.h"
#include "Simulation.h"
#include "pch.h"

Simulation* FactoryCudaSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new HostCudaSimulation(size, diffiusion, viscosity, dt);
}