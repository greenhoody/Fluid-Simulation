#pragma once
#include "Simulation.h"
#include"pch.h"

class NotEditedSimulation : public Simulation
{
public:
	NotEditedSimulation(int size, float diffiusion, float viscosity, float dt);
	~NotEditedSimulation();
	void NextFrame(float* copy_array) ;
	void AddDensity(int x, int y, float density) ;
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
private:
	void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt);
	void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt);
	void project(int N, float* u, float* v, float* p, float* div);
	void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt);
	void diffuse(int N, int b, float* x, float* x0, float diff, float dt);
	void set_bnd(int N, int b, float* x);
	void add_source(int N, float* x, float* s, float dt);
};