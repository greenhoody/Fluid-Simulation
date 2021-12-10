#include <algorithm>
#include <iterator>
#include <cstring>
#include "pch.h"
#include "Simulation.h"
#include "Simulator.h"
#include "windows.h"
#include "debugapi.h"

#define IX(i,j) ((i)+(simulator->width2)*(j))

Simulation::Simulation(int height, int width, float viscosity, float diffusion) {
	simulator = new Simulator(height, width, viscosity, diffusion);
	tmp = (float*)malloc((unsigned long long)simulator->width2 * (unsigned long long)simulator->height2 * sizeof(float));
}

void Simulation::GetNextFrame(float* density,  float dt) {
	if (density == nullptr) {
		return;
	}
	simulator->NextFrame(dt);
	for (int i = 0; i < simulator->width2; i++) {
		for (int j = 0; j < simulator->height2; j++) {
			if (simulator->boundaries[IX(i, j)]) {
				density[IX(i, j)] = -1;
			}
			else {
				density[IX(i, j)] = simulator->dens[IX(i, j)];

				//TCHAR s[256];
				//swprintf(s,256, __TEXT("Density at %i , %i is %i \n"), i, j, simulator->dens[IX(i, j)]);
				//OutputDebugString(s);

			}
		}
	}
	return;
}

void Simulation::AddSource(int x, int y, float s) {
	simulator->Add_Source(x, y, s);
}

