#pragma once
#include "Simulation.h"
#include "pch.h"

class NotEditedSimulation : public Simulation
{
public:
	float* u, * v, * u_prev, * v_prev, * u_const, * v_const, * dens, * dens_prev, * dens_const;

	NotEditedSimulation(int size, float diffiusion, float viscosity, float dt);
	~NotEditedSimulation();
	void NextFrame(std::shared_ptr<float[]> copy_array) ;
	void AddDensity(int x1, int x2, int y1, int y2, float density);
	void AddVelocity(int x, int y, int r, float v_velocity, float h_velocity);
private:
	void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt);
	void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt);
	void project(int N, float* u, float* v, float* p, float* div);
	void advect(int N, int b, float* d, float* d0, float* u, float* v, float dt);
	void diffuse(int N, int b, float* x, float* x0, float diff, float dt);
	void set_bnd(int N, int b, float* x);
	void add_source(int N, float* x, float* s, float dt);
	//testowa
	void print(float* x, std::string prefix);


};