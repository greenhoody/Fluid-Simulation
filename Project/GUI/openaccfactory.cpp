#include "openaccfactory.h"
#include "simulation.h"
#include "openaccsimulation.h"

Simulation* OpenACCFactory::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
    return new OpenACCSimulation(size, diffiusion, viscosity, dt);
}
