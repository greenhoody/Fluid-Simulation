#include "pch.h"
#include "Simulation2.h"
#include "NotEdited.h"
#include <corecrt_malloc.h>

#define IX(i,j) ((i)+(size+2)*(j))

Simulation2::Simulation2(int size, float diffiusion, float viscosity, float dt) {
	this->size = size;
	this->diff = diffiusion;
	this->visc = viscosity;
	this->dt = dt;

	this->u = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->u_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->v = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->v_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->dens = (float*)calloc((size + 2) * (size + 2), sizeof(float));
	this->dens_prev = (float*)calloc((size + 2) * (size + 2), sizeof(float));
}

void Simulation2::FreeSimulation2() {
	free(u);
	free(u_prev);
	free(v);
	free(v_prev);
	free(dens);
	free(dens_prev);
}

float* Simulation2::NextFrame() {
	vel_step(size, u, v, u_prev, v_prev, visc, dt);
	dens_step(size, dens, dens_prev, u, v, diff, dt);
	return dens;
}

void Simulation2::AddDensity(int x, int y, float density) {
	dens[IX(x + 1, y + 1)] += density;
}

void Simulation2::AddVelocity(int x, int y, float v_velocity, float h_velocity) {
	v[IX(x + 1, y + 1)] += v_velocity;
	u[IX(x + 1, y + 1)] += h_velocity;
}