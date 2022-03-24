//#pragma once

#include <iostream>

#ifndef SIMULATION_HEADER
#define SIMULATION_HEADER

class Simulation
{
public:
	virtual void NextFrame(std::shared_ptr<float[]> copy_array) = 0;
	virtual void AddDensity(int x, int y, float density) = 0;
	virtual void AddVelocity(int x, int y, float v_velocity, float h_velocity) = 0;
	virtual void AddConstantDensity(int x, int y, float density) = 0;
	virtual void DeleteConstantDensity(int x, int y) = 0;
	virtual void AddConstantVelocity(int x, int y, float v_velocity, float h_velocity) = 0;
	virtual void DeleteConstantVelocity(int x, int y) = 0;
	virtual ~Simulation() {
		//free(u);
		//free(u_prev);
		//free(u_const);
		//free(v);
		//free(v_prev);
		//free(v_const);
		//free(dens);
		//free(dens_prev);
		//free(dens_const);
	};
	Simulation(int size, float diffiusion, float viscosity, float dt) {
		this->size = size;
		this->diff = diffiusion;
		this->visc = viscosity;
		this->dt = dt;

		int n = (size + 2) * (size + 2);

		u.reset((float*)calloc(n, sizeof(float)));
		v.reset((float*)calloc(n, sizeof(float)));
		u_prev.reset((float*)calloc(n, sizeof(float)));
		v_prev.reset((float*)calloc(n, sizeof(float)));
		u_const.reset((float*)calloc(n, sizeof(float)));
		v_const.reset((float*)calloc(n, sizeof(float)));
		dens.reset((float*)calloc(n, sizeof(float)));
		dens_prev.reset((float*)calloc(n, sizeof(float)));
		dens_const.reset((float*)calloc(n, sizeof(float)));

		//this->u_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->u_const = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->v = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->v_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->v_const = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->dens = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->dens_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
		//this->dens_const = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	};
	std::shared_ptr<float[]> u, v, u_prev, v_prev, u_const, v_const, dens, dens_prev, dens_const;
	//float* u, * v, * u_prev, * v_prev, * u_const, * v_const, * dens, * dens_prev, * dens_const;
	int size;
	float diff, visc, dt;
};

#endif