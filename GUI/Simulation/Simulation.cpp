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
}

void Simulation::GetNextFrame(float* density,  float dt) {
	simulator->NextFrame(dt);
	float* tmp = (float*)malloc((unsigned long long)simulator->width2 * (unsigned long long)simulator->height2 * sizeof(float));
	for (int i = 0; i < simulator->width2; i++) {
		for (int j = 0; j < simulator->height2; j++) {
			if (simulator->boundaries[IX(i, j)]) {
				tmp[IX(i, j)] = -1;
			}
			else {
				tmp[IX(i, j)] = simulator->dens[IX(i, j)];

				//TCHAR s[256];
				//swprintf(s,256, __TEXT("Density at %i , %i is %i \n"), i, j, simulator->dens[IX(i, j)]);
				//OutputDebugString(s);

			}
		}
	}

	//std::copy(tmp, tmp + (simulator->width2 * simulator->height2 * sizeof(float)), density);
	memcpy(density, tmp, sizeof(tmp));
	free(tmp);
	return;
}

void Simulation::AddSource(int x, int y, float s) {
	simulator->Add_Source(x, y, s);
}

