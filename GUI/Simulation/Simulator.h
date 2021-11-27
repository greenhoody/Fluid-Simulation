#pragma once

class Simulator {
// 
public:
	float * hVelocity, * vVelocity, * vPrev, * hPrev, * dens, * densPrev, * densSources;
	bool* boundaries;

private:
	int size;
	int height, width;
	// height and widht of simulation with added boundaries
	int height2, width2;
	double viscosity, diffusion;

// methods
public:
	Simulator(int height, int widht, float viscosity, float diffusion);
	void Add_Source(int x, int y, float quantity);
	float* NextFrame(float dt);

private:
	
	void bnd(int b, float* x);
	void source(float* grid, float* sources, float dt);
	void diffuse(int b, float* grid, float* grid2, float diff, float dt);
	void advect(int b, float* grid, float* grid2, float* h, float* v, float dt);
	void dens_step(float* x, float* x0, float* h, float* v, float diff, float dt);
	void vel_step(float* h, float* v, float* h0, float* v0, float visc, float dt);
	void project(float* h, float* v, float* h0, float* v0);
};
