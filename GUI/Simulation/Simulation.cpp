#include "pch.h"
#include "Simulation.h"
#include "Simulator.h"

Simulation::Simulation(int height, int widht, float viscosity, float diffusion) {
	simulator = new Simulator(height, widht, viscosity, diffusion);
}

float* Simulation::GetNextFrame(float dt) {
	return simulator->NextFrame(dt);
}
