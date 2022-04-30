#include "noteditedfactory.h"
#include "simulation.h"
#include "noteditedsimulation.h"

Simulation *NotEditedFactory::CreateSimulation(int size, float diffiusion, float viscosity, float dt)
{
    return new NotEditedSimulation(size, diffiusion, viscosity, dt);
}
