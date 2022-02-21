#pragma once
#include "Simulation.h"

class Factory
{
	virtual Simulation CreateSimulation() = 0;
};
