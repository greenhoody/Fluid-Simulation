#pragma once

class Simulation
{
public:
	float* u, * v, * u_prev, * v_prev, * dens, * dens_prev;
	int size;
	float diff, visc, dt;

	void NextFrame(float* copy_array) = 0;
	void AddDensity(int x, int y, float density) = 0;
	void AddVelocity(int x, int y, float v_velocity, float h_velocity) = 0;

};