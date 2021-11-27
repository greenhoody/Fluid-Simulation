#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "pch.h"
#include "Simulation.h"
#include "Simulator.h"

#define IX(i,j) ((i)+(simulator->width2)*(j))

Simulation::Simulation(int height, int widht, float viscosity, float diffusion) {
	simulator = new Simulator(height, widht, viscosity, diffusion);
}

void Simulation::GetNextFrame(float* density,  float dt) {
	simulator->NextFrame(dt);
	float* tmp = (float*)malloc(simulator->width2 * simulator->height2 * sizeof(float));
	for (int i = 0; i < simulator->width2; i++) {
		for (int j = 0; j < simulator->height2; j++) {
			if (simulator->boundaries[IX(i, j)]) {
				tmp[IX(i, j)] = -1;
			}
			else {
				tmp[IX(i, j)] = simulator->dens[IX(i, j)];
			}
		}
	}
	
	std::copy(tmp, tmp + (simulator->width2 * simulator->height2 * sizeof(float)), density);
	free(tmp);
	return;
}

