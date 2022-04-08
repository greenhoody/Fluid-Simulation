#include "FactoryCudaSimulation.h"
#include "HostCudaSimulation.cuh"
#include "Simulation.h"
#include "pch.h"

Simulation* FactoryCudaSimulation::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new HostCudaSimulation(size, diffiusion, viscosity, dt);
}