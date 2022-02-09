#include "pch.h"
#include "Simulation2.h"
#include "NotEdited.h"
#include <corecrt_malloc.h>
#include <string.h>
#include <algorithm>

#define IX(i,j) ((i+1)+(size+2)*(j+1))

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

void Simulation2::NextFrame(float* copy_array) {
	vel_step(size, u, v, u_prev, v_prev, visc, dt);
	dens_step(size, dens, dens_prev, u, v, diff, dt);
	// to wywala b³¹d
	//copy_array.insert(copy_array.end(),dens[0], &dens[(size + 2) * (size + 2) -1])
	//std::copy(&dens[0], &dens[(size + 2)*(size+2)-1],copy_array);
	memcpy(copy_array, dens, sizeof(float) * (size + 2) * (size + 2));
}

void Simulation2::AddDensity(int x, int y, float density) {
	dens[IX(x, y)] += density;
}

void Simulation2::AddVelocity(int x, int y, float v_velocity, float h_velocity) {
	v[IX(x , y)] += v_velocity;
	u[IX(x , y)] += h_velocity;
}