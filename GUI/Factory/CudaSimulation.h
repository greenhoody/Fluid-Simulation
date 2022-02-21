#pragma once
#include "Simulation.h"

class CudaSimulation : public Simulation
{
public:
	void NextFrame(float* copy_array) = 0;
	void AddDensity(int x, int y, float density) = 0;
	void AddVelocity(int x, int y, float v_velocity, float h_velocity) = 0;
};