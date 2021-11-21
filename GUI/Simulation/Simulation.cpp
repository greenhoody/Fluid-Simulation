#include "pch.h"
#include "Simulation.h"
#include "Simulator.h"

Simulation::Simulation(int height, int width) {
	simulator = new Simulator(height, width);
}
