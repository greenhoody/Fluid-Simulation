#pragma once
#include "Simulation.h"

class EditedSimulation : public Simulation {
public :
	EditedSimulation(int size, float diffiusion, float viscosity, float dt);
	~EditedSimulation();
	void NextFrame(float* copy_array);
	void AddDensity(int x, int y, float density);
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
	void AddWall(int x, int y);
	void DeleteWall(int x, int y);
	void AddConstantDensity(int x, int y, float density);
	void DeleteConstantDensity(int x, int y);
	void AddConstantVelocity(int x, int y, float v_velocity, float h_velocity);
	void DeleteConstantVelocity(int x, int y);
private:
	float* source;
	bool* walls;

	void vel_step(float* u, float* v, float* u0, float* v0, float visc);
	void dens_step(float* x, float* x0, float* u, float* v, float diff);
	void project(float* u, float* v, float* p, float* div);
	void advect(int b, float* d, float* d0, float* u, float* v);
	void diffuse(int b, float* x, float* x0, float diff);
	void set_bnd(int b, float* x);
	void constant (float* x, float* c);
};