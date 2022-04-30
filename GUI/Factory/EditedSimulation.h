#pragma once
#include "Simulation.h"

class EditedSimulation : public Simulation {
public :
	float* u, * v, * u_prev, * v_prev, * u_const, * v_const, * dens, * dens_prev, * dens_const;

	EditedSimulation(int size, float diffiusion, float viscosity, float dt);
	~EditedSimulation();
	void NextFrame(std::shared_ptr<float[]> copy_array);
	void AddDensity(int x1, int x2, int y1, int y2, float density);
	void AddVelocity(int x, int y, int r, float v_velocity, float h_velocity);
	void AddWall(int x, int y);
	void DeleteWall(int x, int y);
private:
	std::unique_ptr<bool[]> walls;

	void vel_step(float* u, float* v, float* u0, float* v0, float visc);
	void dens_step(float* x, float* x0, float* u, float* v, float diff);
	void project(float* u, float* v, float* p, float* div);
	void advect(int b, float* d, float* d0, float* u, float* v);
	void diffuse(int b, float* x, float* x0, float diff);
	void set_bnd(int b, float* x);
	void constant (float* x, float* c);
	void constant_no_limit(float* x, float* c);
};