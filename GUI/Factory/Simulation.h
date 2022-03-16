//#pragma once

#include <iostream>

#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

class Simulation
{
public:
	virtual void NextFrame(float* copy_array) = 0;
	virtual void AddDensity(int x, int y, float density) = 0;
	virtual void AddVelocity(int x, int y, float v_velocity, float h_velocity) = 0;

	virtual ~Simulation() {};

	float* u, * v, * u_prev, * v_prev, * dens, * dens_prev;
	int size;
	float diff, visc, dt;
};

#endif