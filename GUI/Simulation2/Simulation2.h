#pragma once

#include "NotEdited.h"
#include <vector>

class Simulation2
{
public:
	float *u, *v, *u_prev, *v_prev, *dens, *dens_prev;
	int size;
	float diff, visc, dt;

	Simulation2(int size, float diffiusion, float viscosity, float dt);
	void FreeSimulation2();
	void NextFrame(float* copy_array);
	void AddDensity(int x, int y, float density);
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
};

