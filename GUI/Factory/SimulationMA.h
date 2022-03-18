#include "Simulation.h"
#include"pch.h"

class SimulationMA : public Simulation {
public:
	SimulationMA(int size, float diffiusion, float viscosity, float dt);
	~SimulationMA();
	void NextFrame(float* copy_array);
	void AddDensity(int x, int y, float density);
	void AddVelocity(int x, int y, float v_velocity, float h_velocity);
private:
	void vel_step(int N, float* u, float* v, float* u0, float* v0, float visc, float dt);
	void dens_step(int N, float* x, float* x0, float* u, float* v, float diff, float dt);
	void project(float* u, float* v, float* p, float* div, int iter, int N);
	void advect( int b, float* d, float* d0, float* u, float* v, float dt, int N);
	void diffuse(int b, float* x, float* x0, float diff, float dt, int iter, int N);
	void set_bnd(int b, float* x, int N);
	void add_source(int N, float* x, float* s, float dt);

	// do testowania
	void lin_solve(int b, float* x, float* x0, float a, float c, int iter, int N);

	//testowa
	void print(float* x, std::string prefix);
};