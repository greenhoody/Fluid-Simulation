#include "cudafactory.h"
#include "../CUDA/HostCudaSimulation.h"
#include "simulation.h"

Simulation* CudaFactory::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
	return new HostCudaSimulation(size, diffiusion, viscosity, dt);
}
