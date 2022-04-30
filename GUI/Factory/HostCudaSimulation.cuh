#pragma once
#include "Simulation.h"
#include "pch.h"

#include <cuda_runtime_api.h> 
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

class HostCudaSimulation : public Simulation
{
public:
	int thread_no, block_no;
	float* u, * v, * u_prev, * v_prev, * u_const, * v_const, * dens, * dens_prev, * dens_const;

	HostCudaSimulation(int size, float diffiusion, float viscosity, float dt);
	~HostCudaSimulation();
	void NextFrame(std::shared_ptr<float[]> copy_array);
	void AddDensity(int x1, int x2, int y1, int y2, float density);
	void AddVelocity(int x, int y, int r, float v_velocity, float h_velocity);

};